#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "Pathfinding.h"

using namespace AIForGames;

int main()
{
    // Setup
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    // Init
    Node* a = new Node();
    a->position = glm::vec2(125.0f, 75.0f);
    Node* b = new Node();
    b->position = glm::vec2(250.0f, 75.0f);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        {

        }
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
