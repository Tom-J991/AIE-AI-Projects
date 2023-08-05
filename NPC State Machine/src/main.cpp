#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "Pathfinding.h"
#include "NodeMap.h"
#include "Agent.h"

#include "Behaviours/UtilityAI.h"
#include "Behaviours/FiniteStateMachine.h"
#include "Behaviours/State.h"

#include "Behaviours/Wander/WanderBehaviour.h"
#include "Behaviours/PlayerMovement/PlayerMovementBehaviour.h"

#include "Behaviours/Conditions/Distance/DistanceCondition.h"

using namespace AIForGames;

void DrawString(Texture fontTexture, Vector2 position, std::string string, Color color);
void DrawCharacter(Texture fontTexture, Vector2 position, const char character, Color color);

int main()
{
    // Setup
    const int gameWidth = 224;
    const int gameHeight = 288;

    const int renderWidth = 1280;
    const int renderHeight = 720;

    float margins = 48 * ((float)renderHeight / 720);
    float resolution = ((float)renderHeight - ((float)margins*2)) / (float)gameHeight;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(renderWidth, renderHeight, "Pac-Man");
    SetTargetFPS(60);

    RenderTexture2D framebuffer = LoadRenderTexture(gameWidth, gameHeight);

    Shader postProcessingShader = LoadShader(0, TextFormat("./res/shaders/postprocess.fs", 330));

    float gw = (float)gameWidth;
    float gh = (float)gameHeight;
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "renderWidth"), &renderWidth, SHADER_UNIFORM_INT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "renderHeight"), &renderHeight, SHADER_UNIFORM_INT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "gameWidth"), &gw, SHADER_UNIFORM_FLOAT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "gameHeight"), &gh, SHADER_UNIFORM_FLOAT);
    SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);

    // Init
    NodeMap nodeMap;

    // TODO: Implement Stage Builder using "stage_parts" sprite sheet, 
    // in the original arcade game every level was the same stage but I eventually want stages to be easily created and customizable.
    std::vector<std::string> asciiMap;
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0111111111111001111111111110");
    asciiMap.push_back("0100001000001001000001000010");
    asciiMap.push_back("0100001000001001000001000010");
    asciiMap.push_back("0100001000001001000001000010");
    asciiMap.push_back("0111111111111111111111111110");
    asciiMap.push_back("0100001001000000001001000010");
    asciiMap.push_back("0100001001000000001001000010");
    asciiMap.push_back("0111111001111001111001111110");
    asciiMap.push_back("0000001000001001000001000000");
    asciiMap.push_back("0000001000001001000001000000");
    asciiMap.push_back("0000001001111111111001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("1111111111000000001111111111");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0000001001111111111001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0111111111111001111111111110");
    asciiMap.push_back("0100001000001001000001000010");
    asciiMap.push_back("0100001000001001000001000010");
    asciiMap.push_back("0111001111111111111111001110");
    asciiMap.push_back("0001001001000000001001001000");
    asciiMap.push_back("0001001001000000001001001000");
    asciiMap.push_back("0111111001111001111001111110");
    asciiMap.push_back("0100000000001001000000000010");
    asciiMap.push_back("0100000000001001000000000010");
    asciiMap.push_back("0111111111111111111111111110");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");

    nodeMap.SetCellSize(8); // Tiles are 8x8
    nodeMap.Initialize(asciiMap);

    Node *playerStart = nodeMap.GetNode(14, 20);

    // https://www.gamedeveloper.com/design/the-pac-man-dossier
    // https://gameinternals.com/understanding-pac-man-ghost-behavior

    // Pac-Man's Behaviour
    // 
    // Pac-Man starts with 5 lives.
    // When Pac-Man eats a pellet, it's 10 points, a super-pellet is 50 points.
    // Consuming the super-pellet changes the state of every ghost in the maze to their frightened state and allowing him to eat the ghosts.
    // Eating the ghosts gains additional points, 200 points at first, 400, 800, then 1,600 as he eats more ghosts.
    // Eating all the ghosts before the super-pellet runs out can reward an additional 12,000 points on earlier levels.
    // Pac-Man dies and loses a life if a ghost manages to catch him.
    // Pac-Man will only die if the ghost reaches the center point of the current node Pac-Man is in.
    // //
    // After 70 pellets have been eaten on the first level, 170 pellets on the second level, a fruit/item will appear in the middle of the stage.
    // Getting the fruit or item rewards anywhere from 100 to 5,000 points depending on the current level.
    // After a certain duration if the fruit/item is not collected it will disappear, the exact duration is a random variable between 9 to 10 seconds.
    //
    Agent player(&nodeMap, new PlayerMovementBehaviour());
    player.SetNode(playerStart);
    player.SetColor(YELLOW);
    player.SetSize(8);
    player.SetSpeed(88); // Pac-Man always moves 11 tiles per second, (tiles are 8x8, so 11*8 = 88).

    // Ghost's AI
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
    Agent blinky(&nodeMap, new WanderBehaviour());
    blinky.SetNode(nodeMap.GetRandomNode());
    blinky.SetColor(RED);
    blinky.SetSize(8);
    blinky.SetSpeed(88 * 0.75f);

    // When Pinky (Pink) leaves the ghost house, she chases the spot 4 tiles in front of Pac-Man, 
    // an overflow error in the original game causes it to be 4 tiles in front of and 4 tiles to the left of Pac-Man if he is facing upwards.
    // Pinky's desginated corner is the upper-left.
    // Pinky starts in the ghost house but always immediately exits.
    Agent pinky(&nodeMap, new WanderBehaviour());
    pinky.SetNode(nodeMap.GetRandomNode());
    pinky.SetColor(PINK);
    pinky.SetSize(8);
    pinky.SetSpeed(88 * 0.75f);

    // When Inky (Blue) leaves the ghost house, his target is complicated.
    // Inky's target is found by creating a vector from Blinky's position to the spot 2 tiles in front of Pac-Man,
    // then doubling the length of that vector, where that vector is pointing to now is Inky's target. 
    // Inky's designated corner is the lower-right.
    // Inky starts in the ghost house and exits when Pac-Man has consumed at least 30 pellets.
    Agent inky(&nodeMap, new WanderBehaviour());
    inky.SetNode(nodeMap.GetRandomNode());
    inky.SetColor(SKYBLUE);
    inky.SetSize(8);
    inky.SetSpeed(88 * 0.75f);

    // When Clyde (Orange) leaves the ghost house, he chases directly after Pac-Man but scatters to his corner when within an 8 tile radius of Pac-Man,
    // Clyde's designated corner is the lower-left.
    // Clyde is the last to exit the ghost house and doesn't exit at all in the first level until over a third of the pellets in the stage have been consumed.
    Agent clyde(&nodeMap, new WanderBehaviour());
    clyde.SetNode(nodeMap.GetRandomNode());
    clyde.SetColor(ORANGE);
    clyde.SetSize(8);
    clyde.SetSpeed(88 * 0.75f);

    int playerScore = 00;

    Texture fontTex = LoadTexture("./res/sprites/font/font-formatted.png");

    Texture rgbTex = LoadTexture("./res/sprites/postprocess/rgb.png");
    Texture bezelTex = LoadTexture("./res/sprites/bezel/pacman_bezel.png");
    GenTextureMipmaps(&bezelTex);
    SetTextureFilter(bezelTex, TEXTURE_FILTER_TRILINEAR);

    Texture stageTex = LoadTexture("./res/sprites/stage/stage.png"); // TODO: Draw stage from parts rather than from a full image.

    Texture pacmanTex = LoadTexture("./res/sprites/pacman.png");
    Texture blinkyTex = LoadTexture("./res/sprites/blinky.png");
    Texture pinkyTex = LoadTexture("./res/sprites/pinky.png");
    Texture inkyTex = LoadTexture("./res/sprites/inky.png");
    Texture clydeTex = LoadTexture("./res/sprites/clyde.png");

    // Main game loop
    float time = (float)GetTime();
    float deltaTime;
    while (!WindowShouldClose())
    {
        // Calculate timings
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        // Update
        {
            player.Update(deltaTime);
            blinky.Update(deltaTime);
            pinky.Update(deltaTime);
            inky.Update(deltaTime);
            clyde.Update(deltaTime);
        }

        // Draw to Framebuffer
        BeginTextureMode(framebuffer);
        ClearBackground(BLACK);
        {
            // Draw Debug
            nodeMap.Draw();

            DrawPath(blinky.GetPath(), { 255, 0, 0, 255 });
            DrawPath(pinky.GetPath(), { 255, 0, 255, 255 });
            DrawPath(inky.GetPath(), { 0, 255, 255, 255 });
            DrawPath(clyde.GetPath(), { 255, 128, 64, 255 });
            DrawPath(player.GetPath(), { 255, 255, 0, 255 });

            // Draw Game
            DrawTexturePro(pacmanTex, 
                { 1 * 16, 0 * 16, 16, 16 },
                { player.GetPosition().x - 8, player.GetPosition().y - 8, 16, 16 },
                { 0, 0 },
                0, WHITE); // Temp

            DrawTexturePro(blinkyTex,
                { 0 * 16, 0 * 16, 16, 16 },
                { blinky.GetPosition().x - 8, blinky.GetPosition().y - 8, 16, 16 },
                { 0, 0 },
                0, WHITE); // Temp
            DrawTexturePro(pinkyTex,
                { 0 * 16, 0 * 16, 16, 16 },
                { pinky.GetPosition().x - 8, pinky.GetPosition().y - 8, 16, 16 },
                { 0, 0 },
                0, WHITE); // Temp
            DrawTexturePro(inkyTex,
                { 0 * 16, 0 * 16, 16, 16 },
                { inky.GetPosition().x - 8, inky.GetPosition().y - 8, 16, 16 },
                { 0, 0 },
                0, WHITE); // Temp
            DrawTexturePro(clydeTex,
                { 0 * 16, 0 * 16, 16, 16 },
                { clyde.GetPosition().x - 8, clyde.GetPosition().y - 8, 16, 16 },
                { 0, 0 },
                0, WHITE); // Temp

            DrawTextureEx(stageTex, { 0, 24 }, 0, 1, WHITE); // Draw stage.

            // Draw UI
            const Color uiColor = { 224, 221, 255, 255 };
            DrawString(fontTex, { 9, 0 }, "HIGH SCORE", uiColor);

            std::stringstream scoreStream;
            scoreStream << std::setw(2) << std::setfill('0') << playerScore;
            DrawString(fontTex, { 5, 1 }, scoreStream.str(), uiColor);
        }
        EndTextureMode();

        // Draw to Screen
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

            // Draw Bezel
            DrawTexturePro(bezelTex,
                { 0, 0, (float)bezelTex.width, (float)bezelTex.height },
                { 0, 0, renderWidth, renderHeight },
                { 0, 0 },
                0, WHITE);
        }
        EndDrawing();
    }

    // Cleanup and close
    UnloadTexture(fontTex);
    UnloadTexture(bezelTex);
    UnloadTexture(rgbTex);
    UnloadTexture(stageTex);
    UnloadTexture(pacmanTex);
    UnloadTexture(blinkyTex);
    UnloadTexture(pinkyTex);
    UnloadTexture(inkyTex);
    UnloadTexture(clydeTex);

    UnloadShader(postProcessingShader);

    UnloadRenderTexture(framebuffer);

    CloseWindow();

    return 0;
}

// Not using the regular font rendering because they won't scale down properly and changing the filtering mode doesn't seem to help.
void DrawString(Texture fontTexture, Vector2 position, std::string string, Color color)
{
    std::vector<char> v(string.begin(), string.end());
    for (int i = 0; i < v.size(); ++i)
    {
        Vector2 charPos = { position.x + i, position.y };
        DrawCharacter(fontTexture, charPos, v[i], color);
    }
}

void DrawCharacter(Texture fontTexture, Vector2 position, const char character, Color color)
{
    // Assumes size of each character is 8x8

    int ascii = int(character) - 32; // Starts at ascii code for space.
    // Index to array
    int x = ascii % (fontTexture.width / 8);
    int y = ascii / (fontTexture.width / 8);

    // Texture is in order of ascii code https://www.asciitable.com/
    // Texture probably doesn't support some characters.
    DrawTexturePro(fontTexture, { (float)x * 8, (float)y * 8, 8, 8 }, { position.x * 8, position.y * 8, 8, 8 }, { 0, 0 }, 0, color);
}
