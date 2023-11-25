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

#include "Behaviours/Idle/IdleBehaviour.h"
#include "Behaviours/Wander/WanderBehaviour.h"
#include "Behaviours/Flee/FleeBehaviour.h"
#include "Behaviours/Follow/FollowBehaviour.h"
#include "Behaviours/Attack/AttackBehaviour.h"

#include "Behaviours/Conditions/Condition.h"
#include "Behaviours/Conditions/Distance/DistanceCondition.h"
#include "Behaviours/Conditions/Timer/TimerCondition.h"
#include "Behaviours/Conditions/Health/HealthCondition.h"
#include "Behaviours/Conditions/RandomTimer/RandomTimerCondition.h"
#include "Behaviours/Conditions/FlagCheck/FlagCheckCondition.h"

using namespace AIForGames;

bool musicTrigger = false;

Agent *agent = nullptr;
Agent *agent2 = nullptr;

Game::Game(int difficulty)
{ 
    std::cout << "Chosen Difficulty: " << difficulty << std::endl;
    Globals::g_difficulty = difficulty;

    m_nodeMap = new NodeMap();

    Globals::g_resourceManager = new ResourceManager();

    Globals::g_resourceManager->AddMusic("FIGHT", "./res/bgm/fight_looped.ogg");
    Globals::g_resourceManager->AddMusic("GAMEOVER", "./res/bgm/gameover.ogg");

    Globals::g_resourceManager->AddSound("HIT1", "./res/sfx/hit05.ogg");
    Globals::g_resourceManager->AddSound("HIT2", "./res/sfx/hit07.ogg");
    Globals::g_resourceManager->AddSound("HIT3", "./res/sfx/hit08.ogg");
    Globals::g_resourceManager->AddSound("HIT4", "./res/sfx/hit09.ogg");
    Globals::g_resourceManager->AddSound("HIT5", "./res/sfx/hit10.ogg");
    Globals::g_resourceManager->AddSound("HIT6", "./res/sfx/hit12.ogg");
    Globals::g_resourceManager->AddSound("HIT7", "./res/sfx/hit19.ogg");
    Globals::g_resourceManager->AddSound("HIT8", "./res/sfx/hit22.ogg");
    Globals::g_resourceManager->AddSound("HIT9", "./res/sfx/hit28.ogg");

    Globals::g_resourceManager->AddSound("MISS1", "./res/sfx/miss-3.ogg");
    Globals::g_resourceManager->AddSound("MISS2", "./res/sfx/miss-4.ogg");
    Globals::g_resourceManager->AddSound("MISS3", "./res/sfx/miss-7.ogg");
    Globals::g_resourceManager->AddSound("MISS4", "./res/sfx/miss-8.ogg");
    Globals::g_resourceManager->AddSound("MISS5", "./res/sfx/miss-10.ogg");

    Globals::g_resourceManager->AddSound("CHEER", "./res/sfx/cheer.ogg");
}
Game::~Game()
{ 
    Globals::g_resourceManager->Cleanup();

    delete m_nodeMap;
    delete Globals::g_resourceManager;
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

    m_avatar1Dead = false;
    m_avatar2Dead = false;

    m_avatar1IsAttacking = false;
    m_avatar2IsAttacking = false;

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

    float dist = 2.0f;
    if (Globals::g_difficulty > 1)
    {
        int mult = Globals::g_difficulty-1;
        dist -= 0.5f * mult;
    }

    DistanceCondition* avatar1DistanceClose = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar1DistanceFar = new DistanceCondition(dist * 4 * m_nodeMap->GetCellSize(), false);
    DistanceCondition* avatar2DistanceClose = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2DistanceFar = new DistanceCondition(dist * 4 * m_nodeMap->GetCellSize(), false);

    float time = 4.0f;
    if (Globals::g_difficulty > 1)
    {
        int mult = Globals::g_difficulty-1;
        time -= 0.5f * mult;
    }

    RandomTimerCondition *avatar1Timer = new RandomTimerCondition(1.0f, time);
    RandomTimerCondition *avatar2Timer = new RandomTimerCondition(1.0f, time);

    FlagCheckCondition *avatar1AttackCheck = new FlagCheckCondition(&m_avatar1IsAttacking, false);
    FlagCheckCondition *avatar2AttackCheck = new FlagCheckCondition(&m_avatar2IsAttacking, false);

    HealthCondition *avatar1LowHealth = new HealthCondition(&m_avatar1Health, m_avatar1Health/3, false);
    HealthCondition *avatar2LowHealth = new HealthCondition(&m_avatar2Health, m_avatar2Health/3, false);

    State *avatar1IdleState = new State(new IdleBehaviour());
    State *avatar1WanderState = new State(new WanderBehaviour());
    State *avatar1FleeState = new State(new FleeBehaviour());
    State *avatar1ChaseState = new State(new FollowBehaviour());
    State *avatar1AttackState = new State(new AttackBehaviour(&m_avatar2Health, &m_avatar1IsAttacking));

    State *avatar2IdleState = new State(new IdleBehaviour());
    State *avatar2WanderState = new State(new WanderBehaviour());
    State *avatar2FleeState = new State(new FleeBehaviour());
    State *avatar2ChaseState = new State(new FollowBehaviour());
    State *avatar2AttackState = new State(new AttackBehaviour(&m_avatar1Health, &m_avatar2IsAttacking));

    FiniteStateMachine *avatar1FSM = new FiniteStateMachine(avatar1WanderState);
    avatar1FSM->AddState(avatar1IdleState);
    avatar1FSM->AddState(avatar1WanderState);
    avatar1FSM->AddState(avatar1FleeState);
    avatar1FSM->AddState(avatar1ChaseState);
    avatar1FSM->AddState(avatar1AttackState);

    avatar1IdleState->AddTransition(avatar1Timer, avatar1WanderState);
    avatar1IdleState->AddTransition(avatar1LowHealth, avatar1FleeState);
    avatar1IdleState->AddTransition(avatar1DistanceClose, avatar1AttackState);
    avatar1FleeState->AddTransition(avatar1DistanceFar, avatar1WanderState);
    avatar1WanderState->AddTransition(avatar1Timer, avatar1IdleState);
    avatar1WanderState->AddTransition(avatar1Timer, avatar1ChaseState);
    avatar1ChaseState->AddTransition(avatar1Timer, avatar1WanderState);
    avatar1ChaseState->AddTransition(avatar1DistanceClose, avatar1AttackState);
    avatar1AttackState->AddTransition(avatar1AttackCheck, avatar1WanderState);
    avatar1AttackState->AddTransition(avatar1LowHealth, avatar1FleeState);

    FiniteStateMachine *avatar2FSM = new FiniteStateMachine(avatar2WanderState);
    avatar2FSM->AddState(avatar2IdleState);
    avatar2FSM->AddState(avatar2WanderState);
    avatar2FSM->AddState(avatar2FleeState);
    avatar2FSM->AddState(avatar2ChaseState);
    avatar2FSM->AddState(avatar2AttackState);

    avatar2IdleState->AddTransition(avatar2Timer, avatar2WanderState);
    avatar2IdleState->AddTransition(avatar2LowHealth, avatar2FleeState);
    avatar2IdleState->AddTransition(avatar2DistanceClose, avatar2AttackState);
    avatar2FleeState->AddTransition(avatar2DistanceFar, avatar2WanderState);
    avatar2WanderState->AddTransition(avatar2Timer, avatar2IdleState);
    avatar2WanderState->AddTransition(avatar2Timer, avatar2ChaseState);
    avatar2ChaseState->AddTransition(avatar2Timer, avatar2WanderState);
    avatar2ChaseState->AddTransition(avatar2DistanceClose, avatar2AttackState);
    avatar2AttackState->AddTransition(avatar2AttackCheck, avatar2WanderState);
    avatar2AttackState->AddTransition(avatar2LowHealth, avatar2FleeState);

    agent = new Agent(m_nodeMap, avatar1FSM);
    agent2 = new Agent(m_nodeMap, avatar2FSM);

    agent->SetNode(m_nodeMap->GetRandomNode());
    agent->SetTarget(agent2);
    agent->SetSize(24);
    agent->SetSpeed(256);
    agent->SetColor(PINK);

    agent2->SetNode(m_nodeMap->GetRandomNode());
    agent2->SetTarget(agent);
    agent2->SetSize(24);
    agent2->SetSpeed(256);
    agent2->SetColor(PINK);

    if (Globals::g_difficulty > 2)
    {
        int mult = Globals::g_difficulty-2;
        int speed = GetRandomValue(256, 256+(128*mult));
        agent->SetSpeed(speed);
        speed = GetRandomValue(256, 256+(128*mult));
        agent2->SetSpeed(speed);
    }

    musicTrigger = false;
    m_gameover = false;

    PlayMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
}

void Game::Cleanup()
{
    delete agent;
    delete agent2;

    if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT")))
        StopMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
    else if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("GAMEOVER")))
        StopMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER"));
}

void Game::Update(float deltaTime)
{
    if (m_avatar1Health <= 0)
    {
        m_avatar1Dead = true;
        m_gameover = true;
    }
    if (m_avatar2Health <= 0)
    {
        m_avatar2Dead = true;
        m_gameover = true;
    }

    if (m_gameover == true)
    {
        if (musicTrigger == false)
        {
            if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT")))
                StopMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
            PlayMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER"));
            PlaySound(Globals::g_resourceManager->GetSound("CHEER"));

            musicTrigger = true;
        }
    }

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

    if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT")))
        UpdateMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
    else if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("GAMEOVER")))
        UpdateMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER"));
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
