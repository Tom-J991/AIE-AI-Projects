#include "Game.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

#include "Behaviours/State.h"
#include "Behaviours/FiniteStateMachine.h"

#include "Behaviours/Wander/WanderBehaviour.h"
#include "Behaviours/Follow/FollowBehaviour.h"

#include "Behaviours/Conditions/Condition.h"
#include "Behaviours/Conditions/Distance/DistanceCondition.h"
#include "Behaviours/Conditions/Timer/TimerCondition.h"

using namespace AIForGames;

Agent *agent = nullptr;
Agent *agent2 = nullptr;

Game::Game()
{ 
    m_nodeMap = new NodeMap();

    m_resourceManager = new ResourceManager();
}
Game::~Game()
{ 
    delete agent;
    delete agent2;

    delete m_nodeMap;
    delete m_resourceManager;
}

void Game::Init()
{
    std::vector<std::string> asciiMap;
    asciiMap.push_back("0000000000000000000000000000000000000000");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0111111111111111111111111111111111111110");
    asciiMap.push_back("0000000000000000000000000000000000000000");

    m_nodeMap->SetCellSize(32);
    m_nodeMap->Initialize(asciiMap);

    DistanceCondition* avatar1DistanceClose = new DistanceCondition(7.0f * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar1DistanceFar = new DistanceCondition(7.0f * m_nodeMap->GetCellSize(), false);
    DistanceCondition* avatar2DistanceClose = new DistanceCondition(7.0f * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2DistanceFar = new DistanceCondition(7.0f * m_nodeMap->GetCellSize(), false);

    TimerCondition *avatar1Timer = new TimerCondition(4.0f);
    TimerCondition *avatar2Timer = new TimerCondition(4.0f);

    State *avatar1wanderState = new State(new WanderBehaviour());
    State *avatar1followState = new State(new FollowBehaviour());
    avatar1wanderState->AddTransition(avatar1Timer, avatar1followState);
    avatar1wanderState->AddTransition(avatar1DistanceClose, avatar1followState);
    avatar1followState->AddTransition(avatar1Timer, avatar1wanderState);
    avatar1followState->AddTransition(avatar1DistanceFar, avatar1wanderState);

    State *avatar2wanderState = new State(new WanderBehaviour());
    State *avatar2followState = new State(new FollowBehaviour());
    avatar2wanderState->AddTransition(avatar2Timer, avatar2followState);
    avatar2wanderState->AddTransition(avatar2DistanceClose, avatar2followState);
    avatar2followState->AddTransition(avatar2Timer, avatar2wanderState);
    avatar2followState->AddTransition(avatar2DistanceFar, avatar2wanderState);

    FiniteStateMachine *avatar1FSM = new FiniteStateMachine(avatar1wanderState);
    avatar1FSM->AddState(avatar1wanderState);
    avatar1FSM->AddState(avatar1followState);

    FiniteStateMachine *avatar2FSM = new FiniteStateMachine(avatar2wanderState);
    avatar2FSM->AddState(avatar2wanderState);
    avatar2FSM->AddState(avatar2followState);

    agent = new Agent(m_nodeMap, avatar1FSM);
    agent2 = new Agent(m_nodeMap, avatar2FSM);

    agent->SetNode(m_nodeMap->GetRandomNode());
    agent->SetTarget(agent2);
    agent->SetSpeed(256);
    agent->SetSize(16);
    agent->SetColor(GREEN);

    agent2->SetNode(m_nodeMap->GetRandomNode());
    agent2->SetTarget(agent);
    agent2->SetSpeed(256);
    agent2->SetSize(16);
    agent2->SetColor(GREEN);
}

void Game::Cleanup()
{
    m_resourceManager->Cleanup();
}

void Game::Update(float deltaTime)
{
    agent->Update(deltaTime);
    agent2->Update(deltaTime);
}

void DrawAvatarHealth(Agent *agent, int &agentHealth)
{
    int offset = 64;
    int w = 64;
    int h = 12;

    int x = agent->GetPosition().x - (w/2);
    int y = agent->GetPosition().y - offset;

    DrawRectangle(x, y, w, h, RED); // Base

    int wh = (int)(w * ((float)agentHealth / 4));

    DrawRectangle(x, y, wh, h, GREEN); // Fill
}

void Game::Draw()
{
    // Draw Debug
    m_nodeMap->Draw();

    // Draw Game
    if (!m_avatar1Dead)
    {
        agent->Draw();
        DrawAvatarHealth(agent, m_avatar1Health);
    }

    if (!m_avatar2Dead)
    {
        agent2->Draw();
        DrawAvatarHealth(agent2, m_avatar2Health);
    }
}
