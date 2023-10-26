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
#include "Behaviours/Attack/AttackBehaviour.h"

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
    delete m_nodeMap;
    delete m_resourceManager;
}

void Game::Init()
{
    std::vector<std::string> asciiMap;
    asciiMap.push_back("000000000000000000000000000");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("011111001111111000000001110");
    asciiMap.push_back("011111001111111000000001110");
    asciiMap.push_back("011111001111111000000001110");
    asciiMap.push_back("011111001111111000000001110");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("011111111111111111111111110");
    asciiMap.push_back("011111111111111000000000000");
    asciiMap.push_back("011111111111111011111111110");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("011111001111111111111111110");
    asciiMap.push_back("000000000000000000000000000");

    m_nodeMap->SetCellSize(48);
    m_nodeMap->Initialize(asciiMap);

    DistanceCondition* avatar1DistanceClose = new DistanceCondition(2.0f * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar1DistanceFar = new DistanceCondition(2.0f * m_nodeMap->GetCellSize(), false);
    DistanceCondition* avatar2DistanceClose = new DistanceCondition(2.0f * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2DistanceFar = new DistanceCondition(2.0f * m_nodeMap->GetCellSize(), false);

    TimerCondition *avatar1Timer = new TimerCondition(2.0f);
    TimerCondition *avatar2Timer = new TimerCondition(2.0f);

    State *avatar1WanderState = new State(new WanderBehaviour());
    State *avatar1FollowState = new State(new FollowBehaviour());
    State *avatar1AttackState = new State(new AttackBehaviour(&m_avatar2Health));
    avatar1WanderState->AddTransition(avatar1Timer, avatar1FollowState);
    avatar1FollowState->AddTransition(avatar1Timer, avatar1WanderState);
    avatar1FollowState->AddTransition(avatar1DistanceClose, avatar1AttackState);
    avatar1AttackState->AddTransition(avatar1DistanceFar, avatar1WanderState);
    avatar1AttackState->AddTransition(avatar1Timer, avatar1WanderState);

    State *avatar2WanderState = new State(new WanderBehaviour());
    State *avatar2FollowState = new State(new FollowBehaviour());
    State *avatar2AttackState = new State(new AttackBehaviour(&m_avatar1Health));
    avatar2WanderState->AddTransition(avatar2Timer, avatar2FollowState);
    avatar2FollowState->AddTransition(avatar2Timer, avatar2WanderState);
    avatar2FollowState->AddTransition(avatar2DistanceClose, avatar2AttackState);
    avatar2AttackState->AddTransition(avatar2DistanceFar, avatar2WanderState);
    avatar2AttackState->AddTransition(avatar2Timer, avatar2WanderState);

    FiniteStateMachine *avatar1FSM = new FiniteStateMachine(avatar1WanderState);
    avatar1FSM->AddState(avatar1WanderState);
    avatar1FSM->AddState(avatar1FollowState);
    avatar1FSM->AddState(avatar1AttackState);

    FiniteStateMachine *avatar2FSM = new FiniteStateMachine(avatar2WanderState);
    avatar2FSM->AddState(avatar2WanderState);
    avatar2FSM->AddState(avatar2FollowState);
    avatar2FSM->AddState(avatar2AttackState);

    agent = new Agent(m_nodeMap, avatar1FSM);
    agent2 = new Agent(m_nodeMap, avatar2FSM);

    agent->SetNode(m_nodeMap->GetRandomNode());
    agent->SetTarget(agent2);
    agent->SetSpeed(256);
    agent->SetSize(24);
    agent->SetColor(GREEN);

    agent2->SetNode(m_nodeMap->GetRandomNode());
    agent2->SetTarget(agent);
    agent2->SetSpeed(256);
    agent2->SetSize(24);
    agent2->SetColor(GREEN);

    m_avatar1Dead = false;
    m_avatar2Dead = false;
    m_avatar1Health = 4;
    m_avatar2Health = 4;
}

void Game::Cleanup()
{
    delete agent;
    delete agent2;

    m_resourceManager->Cleanup();
}

void Game::Update(float deltaTime)
{
    if (m_avatar1Health <= 0)
        m_avatar1Dead = true;
    if (m_avatar2Health <= 0)
        m_avatar2Dead = true;

    if (!m_avatar1Dead)
    {
        agent->Update(deltaTime);
    }

    if (!m_avatar2Dead)
    {
        agent2->Update(deltaTime);
    }

    if (IsKeyPressed(KEY_R))
    {
        Cleanup();
        Init();
    }
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
        DrawText("1", agent->GetPosition().x-MeasureText("1", 24)/2, agent->GetPosition().y-12, 24, BLACK);
        DrawAvatarHealth(agent, m_avatar1Health);
    }

    if (!m_avatar2Dead)
    {
        agent2->Draw();
        DrawText("2", agent2->GetPosition().x-MeasureText("2", 24)/2, agent2->GetPosition().y-12, 24, BLACK);
        DrawAvatarHealth(agent2, m_avatar2Health);
    }

    if (!m_avatar1Dead && m_avatar2Dead)
        DrawText("Avatar 1 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && !m_avatar2Dead)
        DrawText("Avatar 2 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && m_avatar2Dead)
        DrawText("Both Lose! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
}
