#include <iostream>
#include <raylib.h>

#include "Game.h"

int difficulty = 0;

void AskInput()
{
    std::cout << "Enter difficulty (1-4): " << std::endl;
    std::cin >> difficulty;
    if (difficulty <= 0 || difficulty > 4)
    {
        std::cout << "Try again." << std::endl;
        AskInput();
    }
}

int main() // Entry Point
{
    AskInput(); // Difficulty Input

    if (difficulty <= 0)
        return 0;

    // Setup Window
    const int gameWidth = 224; // Game resolution
    const int gameHeight = 288;

    const int renderWidth = 1280; // Window resolution
    const int renderHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Anti-aliasing if available.

    InitWindow(renderWidth, renderHeight, "State Machine");
    SetTargetFPS(60);

    InitAudioDevice();

    // Initialize Game
    Game *game = new Game(difficulty);
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

        // Update Game
        game->Update(deltaTime);

        // Draw to screen
        BeginDrawing();
        ClearBackground(WHITE);
        {
            game->Draw();
        }
        EndDrawing();
    }

    // Cleanup and close
    game->Cleanup();
    delete game;
    game = nullptr;

    CloseWindow();

    CloseAudioDevice();

    return 0;
}
