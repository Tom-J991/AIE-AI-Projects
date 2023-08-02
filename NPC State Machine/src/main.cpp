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

#include "Behaviours/GotoPoint/GotoPointBehaviour.h"
#include "Behaviours/Wander/WanderBehaviour.h"
#include "Behaviours/Follow/FollowBehaviour.h"
#include "Behaviours/Selector/SelectorBehaviour.h"
#include "Behaviours/PlayerMovement/PlayerMovementBehaviour.h"

#include "Behaviours/Conditions/Distance/DistanceCondition.h"

using namespace AIForGames;

int main()
{
    // Setup
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pac-Man");
    SetTargetFPS(60);

    // Init
    NodeMap nodeMap;

    // TODO: Implement Stage Builder using "stage_parts" sprite sheet
    std::vector<std::string> asciiMap;
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
    asciiMap.push_back("0000001001111221111001000000");
    asciiMap.push_back("0000001001000220001001000000");
    asciiMap.push_back("0000001001022222201001000000");
    asciiMap.push_back("4111111111022222201111111114");
    asciiMap.push_back("0000001001022222201001000000");
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

    nodeMap.SetCellSize(16);
    nodeMap.Initialize(asciiMap);

    Node *start = nodeMap.GetNode(1, 1);
    Node *end = nodeMap.GetNode(10, 2);

    Agent agent(&nodeMap, new PlayerMovementBehaviour());
    agent.SetNode(start);
    agent.SetColor(WHITE);
    agent.SetSize(8);

    Agent agent2(&nodeMap, new WanderBehaviour());
    agent2.SetNode(nodeMap.GetRandomNode());
    agent2.SetColor(YELLOW);
    agent2.SetSize(8);

    UtilityAI *utilityAI = new UtilityAI();
    utilityAI->AddBehaviour(new WanderBehaviour());
    utilityAI->AddBehaviour(new FollowBehaviour());

    Agent agent3(&nodeMap, utilityAI);
    agent3.SetNode(nodeMap.GetRandomNode());
    agent3.SetTarget(&agent);
    agent3.SetSpeed(128);
    agent3.SetColor(PINK);
    agent3.SetSize(8);

    Texture stage = LoadTexture("./res/sprites/stage/stage.png");
    

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
            agent.Update(deltaTime);
            agent2.Update(deltaTime);
            agent3.Update(deltaTime);
        }
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        {
            nodeMap.Draw();

            DrawPath(agent2.GetPath(), { 255, 255, 0, 255 });
            DrawPath(agent.GetPath(), { 255, 255, 255, 255 });
            DrawPath(agent3.GetPath(), { 255, 0, 255, 255 });
            agent2.Draw();
            agent.Draw();
            agent3.Draw();

            DrawTextureEx(stage, { 0, 0 }, 0, 2, WHITE);
        }
        EndDrawing();
    }

    UnloadTexture(stage);

    CloseWindow();

    return 0;
}
