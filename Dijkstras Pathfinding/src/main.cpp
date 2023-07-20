#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "Pathfinding.h"
#include "NodeMap.h"
#include "PathAgent.h"

using namespace AIForGames;

int main()
{
    // Setup
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Dijkstras Pathfinding");
    SetTargetFPS(60);

    // Init
    NodeMap nodeMap;
    nodeMap.cellSize = 64;

    std::vector<std::string> asciiMap;
    asciiMap.push_back("000000000000");
    asciiMap.push_back("010111011100");
    asciiMap.push_back("010101110110");
    asciiMap.push_back("010100000000");
    asciiMap.push_back("010111111110");
    asciiMap.push_back("010000001000");
    asciiMap.push_back("011111111110");
    asciiMap.push_back("000000000000");

    nodeMap.Initialize(asciiMap);

    Node *start = nodeMap.GetNode(1, 1);
    Node *end = nodeMap.GetNode(10, 2);
    Color lineColor = { 255, 255, 255, 255 };

    PathAgent agent;
    agent.SetNode(start);
    agent.SetSpeed(256);

    // Main game loop
    float time = (float)GetTime();
    float deltaTime;
    while (!WindowShouldClose())
    {
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        // Update
        {
            if (IsMouseButtonPressed(0))
            {
                Vector2 mousePos = GetMousePosition();
                Node* end = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
                agent.GoToNode(end);
            }

            agent.Update(deltaTime);
        }
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        {
            nodeMap.Draw();
            DrawPath(agent.path, lineColor);
            agent.Draw(16);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
