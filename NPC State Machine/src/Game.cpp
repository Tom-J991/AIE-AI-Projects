#include "Game.h"

#include <iostream>

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "Globals.h"

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

Game::Game(int difficulty)
{ 
    std::cout << "Chosen Difficulty: " << difficulty << std::endl;
    Globals::g_difficulty = difficulty;

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

    float dist = 2.0f;
    if (Globals::g_difficulty > 1)
    {
        int mult = Globals::g_difficulty-1;
        dist -= 0.5f * mult;
    }

    DistanceCondition* avatar1DistanceClose = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar1DistanceFar = new DistanceCondition(dist * m_nodeMap->GetCellSize(), false);
    DistanceCondition* avatar2DistanceClose = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2DistanceFar = new DistanceCondition(dist * m_nodeMap->GetCellSize(), false);

    float time = 2.0f;
    if (Globals::g_difficulty > 1)
    {
        int mult = Globals::g_difficulty-1;
        time -= 0.5f * mult;
    }

    TimerCondition *avatar1Timer = new TimerCondition(time);
    TimerCondition *avatar2Timer = new TimerCondition(time);

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

    if (Globals::g_difficulty > 2)
    {
        int mult = Globals::g_difficulty-2;
        int speed = GetRandomValue(256, 256+(128*mult));
        agent->SetSpeed(speed);
        speed = GetRandomValue(256, 256+(128*mult));
        agent2->SetSpeed(speed);
    }

    m_avatar1Dead = false;
    m_avatar2Dead = false;
    m_avatar1Health = 4;
    m_avatar1MaxHP = 4;
    m_avatar2Health = 4;
    m_avatar2MaxHP = 4;

    if (Globals::g_difficulty > 2)
    {
        int mult = Globals::g_difficulty-2;
        int newHP = GetRandomValue(4, 4+(2*mult));
        m_avatar1Health = newHP;
        m_avatar1MaxHP = newHP;
        newHP = GetRandomValue(4, 4+(2*mult));
        m_avatar2Health = newHP;
        m_avatar2MaxHP = newHP;
    }
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

void DrawAvatarHealth(Agent *agent, int &agentHealth, int &maxHealth)
{
    int offset = 64;
    int w = 64;
    int h = 12;

    int x = agent->GetPosition().x - (w/2);
    int y = agent->GetPosition().y - offset;

    DrawRectangle(x, y, w, h, RED); // Base

    int wh = (int)(w * ((float)agentHealth / maxHealth));

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
        DrawAvatarHealth(agent, m_avatar1Health, m_avatar1MaxHP);
    }

    if (!m_avatar2Dead)
    {
        agent2->Draw();
        DrawText("2", agent2->GetPosition().x-MeasureText("2", 24)/2, agent2->GetPosition().y-12, 24, BLACK);
        DrawAvatarHealth(agent2, m_avatar2Health, m_avatar2MaxHP);
    }

    if (!m_avatar1Dead && m_avatar2Dead)
        DrawText("Avatar 1 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && !m_avatar2Dead)
        DrawText("Avatar 2 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && m_avatar2Dead)
        DrawText("Both Lose! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
}
