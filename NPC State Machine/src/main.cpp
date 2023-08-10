// PAC-MAN� & �Bandai Namco Entertainment Inc. 
// 
// https://www.gamedeveloper.com/design/the-pac-man-dossier
// https://gameinternals.com/understanding-pac-man-ghost-behavior
// https://pacman.fandom.com/wiki/Maze_Ghost_AI_Behaviors


#include <raylib.h>

#include "Globals.h"
#include "Game.h"

int main()
{
    // Setup Window
    const int gameWidth = 224; // Game resolution
    const int gameHeight = 288;

    const int renderWidth = 1280; // Window resolution
    const int renderHeight = 720;

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
            game->Draw(); // Draw Game to Framebuffer
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
    game = nullptr;

    UnloadTexture(Globals::g_fontTex);
    UnloadTexture(bezelTex);
    UnloadTexture(rgbTex);

    UnloadShader(postProcessingShader);

    UnloadRenderTexture(framebuffer);

    CloseWindow();

    CloseAudioDevice();

    return 0;
}
