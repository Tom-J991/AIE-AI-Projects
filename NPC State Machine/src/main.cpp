#include <raylib.h>

#include "Globals.h"
#include "Game.h"

int main()
{
    // Setup Window
    const int gameWidth = 224; // Game resolution
    const int gameHeight = 288;

    const int renderWidth = 1920; // Window resolution
    const int renderHeight = 1080;

    float margins = 48 * ((float)renderHeight / 720); // Variables to help resize game to fit into window + bezel overlay.
    float resolution = ((float)renderHeight - ((float)margins*2)) / (float)gameHeight; 

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Anti-aliasing if available.

    InitWindow(renderWidth, renderHeight, "Pac-Man");
    SetTargetFPS(60);

    InitAudioDevice();

    RenderTexture2D framebuffer = LoadRenderTexture(gameWidth, gameHeight); // Create and setup Framebuffer to draw game onto.
    GenTextureMipmaps(&framebuffer.texture);
    SetTextureFilter(framebuffer.texture, TEXTURE_FILTER_POINT);
    SetTextureWrap(framebuffer.texture, TEXTURE_WRAP_CLAMP);

    Shader postProcessingShader = LoadShader(0, TextFormat("./res/shaders/postprocess.fs", 330));

    float gw = (float)gameWidth; // Temp variables for type conversion since SetShaderValue() won't allow it with the parameter.
    float gh = (float)gameHeight;
    // Setup Post-Processing Shader Variables
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "renderWidth"), &renderWidth, SHADER_UNIFORM_INT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "renderHeight"), &renderHeight, SHADER_UNIFORM_INT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "gameWidth"), &gw, SHADER_UNIFORM_FLOAT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "gameHeight"), &gh, SHADER_UNIFORM_FLOAT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);

    Texture rgbTex = LoadTexture("./res/sprites/postprocess/rgb.png");
    GenTextureMipmaps(&rgbTex);
    SetTextureFilter(rgbTex, TEXTURE_FILTER_POINT);
    SetTextureWrap(rgbTex, TEXTURE_WRAP_REPEAT);

    Texture bezelTex = LoadTexture("./res/sprites/bezel/pacman_bezel.png");
    GenTextureMipmaps(&bezelTex);
    SetTextureFilter(bezelTex, TEXTURE_FILTER_TRILINEAR);
    SetTextureWrap(bezelTex, TEXTURE_WRAP_REPEAT);

    Globals::g_fontTex = LoadTexture("./res/sprites/font/font-formatted.png");

    // https://www.gamedeveloper.com/design/the-pac-man-dossier
    // https://gameinternals.com/understanding-pac-man-ghost-behavior

    // Pac-Man's Behaviour
    // 
    // Pac-Man starts with 4 lives.
    // When Pac-Man eats a pellet, it's 10 points, a super-pellet is 50 points.
    // Consuming the super-pellet changes the state of every ghost in the maze to their frightened state and allowing him to eat the ghosts.
    // Eating the ghosts gains additional points, 200 points at first, 400, 800, then 1,600 as he eats more ghosts.
    // Eating all the ghosts before the super-pellet runs out can reward an additional 12,000 points on earlier levels.
    // Pac-Man dies and loses a life if a ghost manages to catch him.
    // Pac-Man will only die if the ghost reaches the center point of the current node Pac-Man is in.
    // As a side-effect of this, Pac-Man can pass through ghosts unfazed in some cases.
    // 
    // After 70 pellets have been eaten on the first level, 170 pellets on the second level, a fruit/item will appear in the middle of the stage.
    // Getting the fruit or item rewards anywhere from 100 to 5,000 points depending on the current level.
    // After a certain duration if the fruit/item is not collected it will disappear, the exact duration is a random variable between 9 to 10 seconds.
    //
    // Ghost's Behaviour
    // 
    // When the game begins, most of the ghosts will stay in the ghost house (The box in the middle) with only one of the ghosts starting in the maze which is usually Blinky,
    // the ghosts will return to the ghost house only after Pac-Man has eaten them and cannot enter it otherwise, Pac-Man himself cannot enter the ghost house.
    // Ghosts will always move to the left once they've left the ghost house, but can immediately reverse direction due to them changing their state.
    // The original arcade game does not use a pathfinding algorithm such as Dijkstras or A*, 
    // instead ghosts will make a pseudorandom decision as soon as it arrives to an intersection to persue a specified target.
    // However, this project will use the A* pathfinding algorithm.
    // 
    // Ghosts are always in one of three possible states: Chase, Scatter, or Frightened.
    // Chase is the state the ghosts spend most their time in, each ghost has a different behaviour when pursuing Pac-Man.
    // The scatter state causes each ghost to scatter to a fixed destination which is typically a specific corner of the stage and loop around that block.
    // In the frightened state, ghosts will constantly move without a target, just pseudorandomly deciding which turn to make when at an intersection.
    // This state turns the ghost blue and allows Pac-Man to eat them.
    // The length of the frightened state is shortened in later levels. Level 19 and onwards effectively removes the state.
    // Ghosts have four waves to alternate between the Chase and Scatter states which are defined for each level, the ghosts start in the scatter state. 
    // The changes between states occur on a fixed timer which resets at the beginning of each level.
    // When the ghosts are in the frightened state, the timer is paused and resumes where it left off after the frightened state is finished, returning to their previous state.
    // In level 1 the waves are defined as:
    // Wave 1 = 7 Seconds Scatter, 20 seconds Chase
    // Wave 2 = 7 Seconds Scatter, 20 seconds Chase
    // Wave 3 = 5 Seconds Scatter, 20 seconds Chase
    // Wave 4 = 5 Seconds Scatter, Chase indefinitely.
    // Levels 2-4 this is defined as:
    // Wave 1 = 7 Seconds Scatter, 20 seconds Chase
    // Wave 2 = 7 Seconds Scatter, 20 seconds Chase
    // Wave 3 = 5 Seconds Scatter, 1033 seconds Chase (Approximately 17 and a half minutes)
    // Wave 4 = 1/60th Seconds Scatter, Chase indefinitely.
    // // Levels 5+ this is defined as:
    // Wave 1 = 5 Seconds Scatter, 20 seconds Chase
    // Wave 2 = 5 Seconds Scatter, 20 seconds Chase
    // Wave 3 = 5 Seconds Scatter, 1037 seconds Chase (Approximately 17 and a half minutes)
    // Wave 4 = 1/60th Seconds Scatter, Chase indefinitely.
    // 
    // Ghosts will never reverse the direction they are going in with the only exception being when they change states, 
    // though changing from their frightened state won't reverse direction.
    //
    // Ghosts by default will always move at 75% of Pac-Man's speed on level 1, and speeding up in later levels.
    // Level 1 = 75% of Pac-Man's speed.
    // Level 2-4 = 85%
    // Level 5+ = 95%
    // Their speed changes though depending on their state
    // 
    // Blinky however will become more aggressive in any level depending on how many pellets are left,
    // speeding up to 85% of Pac-Man's speed on level 1, 105% by level 5 onwards.
    //
    // When Blinky (Red) leaves the ghost house, he follows the player's current tile at all times, as the player eats more pellets he becomes more aggressive and speeds up.
    // Blinky's designated corner is the upper-right.
    // Blinky always starts outside of the ghost house.
    // 
    // On Level 1, Blinky speeds up to 80% of Pac-Man's speed when 20 pellets are left, speeds up to 85% when 10 pellets are left.
    // Level 2, 30 pellets = 90%, 15 pellets = 95%
    // Level 3, 40 pellets = 90%, 20 pellets = 95%
    // Level 4, 40 pellets = 90%, 20 pellets = 95%
    // Level 5+, 40 pellets = 100%, 20 pellets = 105%
    // 
    // After level 5 the max speeds will continue to be 100% and 105% but the amount of pellets left for Blinky to speed up increases,
    // Level 6-8, 50 pellets = 100%, 25 pellets = 105%
    // Level 9-11, 60 pellets = 100%, 30 pellets = 105%
    // Level 12-14, 80 pellets = 100%, 40 pellets = 105%
    // Level 15-18, 100 pellets = 100%, 50 pellets = 105%
    // Level 19-20, 120 pellets = 100%, 60 pellets = 105%
    //
    // When Pinky (Pink) leaves the ghost house, she chases the spot 4 tiles in front of Pac-Man, 
    // an overflow error in the original game causes it to be 4 tiles in front of and 4 tiles to the left of Pac-Man if he is facing upwards.
    // Pinky's desginated corner is the upper-left.
    // Pinky starts in the ghost house but always immediately exits.
    //
    // When Inky (Blue) leaves the ghost house, his target is complicated.
    // Inky's target is found by creating a vector from Blinky's position to the spot 2 tiles in front of Pac-Man,
    // then doubling the length of that vector, where that vector is pointing to now is Inky's target. 
    // Inky's designated corner is the lower-right.
    // Inky starts in the ghost house and exits when Pac-Man has consumed at least 30 pellets.
    //
    // When Clyde (Orange) leaves the ghost house, he chases directly after Pac-Man but scatters to his corner when within an 8 tile radius of Pac-Man,
    // Clyde's designated corner is the lower-left.
    // Clyde is the last to exit the ghost house and doesn't exit at all in the first level until over a third of the pellets in the stage have been consumed.
    //


    // Initialize Game
    Game *game = new Game();
    game->Init();

    // Main game loop
    float time = (float)GetTime();
    float deltaTime;
    while (!WindowShouldClose())
    {
        // Calculate timings
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "time"), &fTime, SHADER_UNIFORM_FLOAT);

        game->Update(deltaTime);

        BeginTextureMode(framebuffer);
        ClearBackground(BLACK);
            game->Draw(); // Draw Game to framebuffer
        EndTextureMode();

        // Draw to screen
        BeginDrawing();
        ClearBackground(BLACK);
        {
            // Draw Framebuffer
            BeginShaderMode(postProcessingShader);
            SetShaderValueTexture(postProcessingShader, GetShaderLocation(postProcessingShader, "texture1"), rgbTex);
            const float width = (float)gameWidth * resolution;
            const float height = (float)gameHeight * resolution;
            DrawTexturePro(framebuffer.texture, 
                { 0, 0, (float)framebuffer.texture.width, -(float)framebuffer.texture.height },
                { ((float)renderWidth - width)/2, ((float)renderHeight - height)/2, width, height },
                { 0, 0 },
                0, WHITE);
            EndShaderMode();

            // Draw Bezel Overlay
            DrawTexturePro(bezelTex,
                { 0, 0, (float)bezelTex.width, (float)bezelTex.height },
                { 0, 0, renderWidth, renderHeight },
                { 0, 0 },
                0, WHITE);
        }
        EndDrawing();
    }

    // Cleanup and close
    game->Cleanup();
    delete game;

    UnloadTexture(Globals::g_fontTex);
    UnloadTexture(bezelTex);
    UnloadTexture(rgbTex);

    UnloadShader(postProcessingShader);

    UnloadRenderTexture(framebuffer);

    CloseWindow();

    CloseAudioDevice();

    return 0;
}
