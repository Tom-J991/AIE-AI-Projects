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

#include "Conditions/Distance/DistanceCondition.h"

using namespace AIForGames;

int main()
{
    // Setup
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "State Machine");
    SetTargetFPS(60);

    // Init
    NodeMap nodeMap;

    std::vector<std::string> asciiMap;
    asciiMap.push_back("0000000000000000000000000");
    asciiMap.push_back("0111011111111111111111110");
    asciiMap.push_back("0111011111111111111111110");
    asciiMap.push_back("0111011000000000000000010");
    asciiMap.push_back("0111011111111101111111110");
    asciiMap.push_back("0111011111111101110000000");
    asciiMap.push_back("0111011111011101111111110");
    asciiMap.push_back("0111011111011100000011110");
    asciiMap.push_back("0111011111011111111111110");
    asciiMap.push_back("0111111111000000000000110");
    asciiMap.push_back("0111101111011111111111110");
    asciiMap.push_back("0111101111011111111111110");
    asciiMap.push_back("0111101111011111111111110");
    asciiMap.push_back("0111101111111111111111110");
    asciiMap.push_back("0000000000000000000000000");

    nodeMap.SetCellSize(32);
    nodeMap.Initialize(asciiMap);

    Node *start = nodeMap.GetNode(1, 1);
    Node *end = nodeMap.GetNode(10, 2);

    Agent agent(&nodeMap, new GotoPointBehaviour());
    agent.SetNode(start);
    agent.SetColor(WHITE);
    agent.SetSize(8);

    Agent agent2(&nodeMap, new WanderBehaviour());
    agent2.SetNode(nodeMap.GetRandomNode());
    agent2.SetColor(YELLOW);
    agent2.SetSize(8);

    /*DistanceCondition *closerThan5 = new DistanceCondition(5.0f * nodeMap.GetCellSize(), true);
    DistanceCondition *furtherThan5 = new DistanceCondition(5.0f * nodeMap.GetCellSize(), false);

    State *wanderState = new State(new WanderBehaviour());
    State *followState = new State(new FollowBehaviour());
    wanderState->AddTransition(closerThan5, followState);
    followState->AddTransition(furtherThan5, wanderState);

    FiniteStateMachine *fsm = new FiniteStateMachine(wanderState);
    fsm->AddState(wanderState);
    fsm->AddState(followState);*/

    UtilityAI *utilityAI = new UtilityAI();
    utilityAI->AddBehaviour(new WanderBehaviour());
    utilityAI->AddBehaviour(new FollowBehaviour());

    Agent agent3(&nodeMap, utilityAI);
    agent3.SetNode(nodeMap.GetRandomNode());
    agent3.SetTarget(&agent);
    agent3.SetSpeed(128);
    agent3.SetColor(PINK);
    agent3.SetSize(8);

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
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
