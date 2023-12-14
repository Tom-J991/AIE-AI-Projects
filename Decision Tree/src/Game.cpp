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

// What a mess.
#include "Decisions/Decision.h"
#include "Decisions/ABDecision.h"

#include "Decisions/Wander/WanderBehaviour.h"
#include "Decisions/Follow/FollowBehaviour.h"
#include "Decisions/Attack/AttackBehaviour.h"

#include "Decisions/Conditions/Condition.h"
#include "Decisions/Conditions/Distance/DistanceCondition.h"
//#include "Decisions/Conditions/Health/HealthCondition.h"
//#include "Decisions/Conditions/Timer/TimerCondition.h"
//#include "Decisions/Conditions/RandomTimer/RandomTimerCondition.h"
//#include "Decisions/Conditions/FlagCheck/FlagCheckCondition.h"

using namespace AIForGames;

bool musicTrigger = false; // Flag to make sure things are only triggered once (because of on update)

Agent *agent = nullptr; // Game avatar agents.
Agent *agent2 = nullptr;

Game::Game(int difficulty)
{ 
    std::cout << "Chosen Difficulty: " << difficulty << std::endl;
    Globals::g_difficulty = difficulty; // Set difficulty as global variable, (Could've also done this in main.cpp but whatever).

    m_nodeMap = new NodeMap(); // Node map for agent navigation.

    // Load and initialize resources.
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
    Globals::g_resourceManager->AddSound("BOO", "./res/sfx/boo.ogg");
}
Game::~Game()
{ 
    Globals::g_resourceManager->Cleanup();

    delete m_nodeMap;
    delete Globals::g_resourceManager;
}

void Game::Init()
{
    // Here we see the simulation's environment as represented by mere numbers encapsulated inside strings.
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

    musicTrigger = false; // Reset flag defaults.
    m_gameover = false;

    // Setup default game variables.
    m_avatar1Dead = false;
    m_avatar2Dead = false;

    m_avatar1IsAttacking = false;
    m_avatar2IsAttacking = false;

    m_avatar1Health = 4;
    m_avatar1MaxHP = 4;
    m_avatar2Health = 4;
    m_avatar2MaxHP = 4;
    if (Globals::g_difficulty > 2) // If difficulty is above 2, randomize the avatar health a bit based on the difficulty.
    {
        int mult = Globals::g_difficulty-2;
        int newHP = GetRandomValue(4, 4+(2*mult));
        m_avatar1Health = newHP;
        m_avatar1MaxHP = newHP;

        newHP = GetRandomValue(4, 4+(2*mult));
        m_avatar2Health = newHP;
        m_avatar2MaxHP = newHP;
    }

    // Setup Decision tree conditions for both avatars.
    float dist = 8.0f;
    if (Globals::g_difficulty > 1) // If difficulty is above 1, randomize the distance checks a bit based on the difficulty.
    {
        int mult = Globals::g_difficulty-1;
        dist -= 0.5f * mult;
    }
    DistanceCondition* avatar1ChaseRange = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar1AttackRange = new DistanceCondition(dist/4 * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2ChaseRange = new DistanceCondition(dist * m_nodeMap->GetCellSize(), true);
    DistanceCondition* avatar2AttackRange = new DistanceCondition(dist/4 * m_nodeMap->GetCellSize(), true);

    // Setup states.
    Decision *avatar1WanderState = new WanderBehaviour();
    Decision *avatar2WanderState = new WanderBehaviour();
    Decision *avatar1FollowState = new FollowBehaviour();
    Decision *avatar2FollowState = new FollowBehaviour();
    Decision *avatar1AttackState = new AttackBehaviour(&m_avatar2Health, &m_avatar1IsAttacking);
    Decision *avatar2AttackState = new AttackBehaviour(&m_avatar1Health, &m_avatar2IsAttacking);

    // Setup decision tree.
    Decision *avatar1AttackDecision = new ABDescision(avatar1AttackState, avatar1FollowState, avatar1AttackRange);
    Decision *avatar2AttackDecision = new ABDescision(avatar2AttackState, avatar2FollowState, avatar2AttackRange);
    Decision *avatar1StartDecision = new ABDescision(avatar1AttackDecision, avatar1WanderState, avatar1ChaseRange);
    Decision *avatar2StartDecision = new ABDescision(avatar2AttackDecision, avatar2WanderState, avatar2ChaseRange);

    // Create avatar agent's and setup default values.
    agent = new Agent(m_nodeMap, avatar1StartDecision);
    agent2 = new Agent(m_nodeMap, avatar2StartDecision);

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

    if (Globals::g_difficulty > 2) // If difficulty is above 2, randomize the avatar's speed a bit based on the difficulty.
    {
        int mult = Globals::g_difficulty-2;
        int speed = GetRandomValue(256, 256+(128*mult));
        agent->SetSpeed(speed);
        speed = GetRandomValue(256, 256+(128*mult));
        agent2->SetSpeed(speed);
    }

    // Play fighting music.
    PlayMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
}

void Game::Cleanup()
{
    delete agent;
    delete agent2;

    // Stop any currently playing music.
    if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT")))
        StopMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
    else if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("GAMEOVER")))
        StopMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER"));
}

void Game::Update(float deltaTime)
{
    if (m_avatar1Health <= 0) // Check if avatar 1 is dead, game over if true.
    {
        m_avatar1Dead = true;
        m_gameover = true;
    }
    if (m_avatar2Health <= 0) // Check if avatar 2 is dead, game over if true.
    {
        m_avatar2Dead = true;
        m_gameover = true;
    }

    if (m_gameover == true) // Trigger music ONCE, on game over.
    {
        if (musicTrigger == false)
        {
            if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT"))) // Stop current music.
                StopMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));

            PlayMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER")); // Play game over music.

            if (m_avatar1Dead && m_avatar2Dead) // Both avatar are dead.
                PlaySound(Globals::g_resourceManager->GetSound("BOO"));
            else // Only one is dead.
                PlaySound(Globals::g_resourceManager->GetSound("CHEER"));

            musicTrigger = true;
        }
    }

    if (!m_avatar1Dead) // Update avatar 1 if not dead.
    {
        agent->Update(deltaTime);
    }
    if (!m_avatar2Dead) // Update avatar 2 if not dead.
    {
        agent2->Update(deltaTime);
    }

    if (IsKeyPressed(KEY_R)) // Reset the simulation when the user presses 'R'
    {
        Cleanup();
        Init();
    }

    // Update Music Streams (required by raylib for some reason).
    if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("FIGHT")))
        UpdateMusicStream(Globals::g_resourceManager->GetMusic("FIGHT"));
    else if (IsMusicStreamPlaying(Globals::g_resourceManager->GetMusic("GAMEOVER")))
        UpdateMusicStream(Globals::g_resourceManager->GetMusic("GAMEOVER"));
}

// Helper function to draw avatar health bars.
void DrawAvatarHealth(Agent *agent, int &agentHealth, int &maxHealth)
{
    int offset = 64; // Settings.
    int w = 64;
    int h = 12;

    int x = agent->GetPosition().x - (w/2); // Find health bar position based on avatar position.
    int y = agent->GetPosition().y - offset;

    DrawRectangle(x, y, w, h, RED); // Base

    int wh = (int)(w * ((float)agentHealth / maxHealth)); // Calculate width of fill based on percentage of avatar health. 
    DrawRectangle(x, y, wh, h, GREEN); // Fill
}

void Game::Draw()
{
    // Draw Debug
    m_nodeMap->Draw();

    // Draw Game
    if (!m_avatar1Dead) // Don't draw avatar 1 if dead.
    {
        agent->Draw();
        DrawText("1", agent->GetPosition().x-MeasureText("1", 24)/2, agent->GetPosition().y-12, 24, BLACK); // Avatar identifier.
        DrawAvatarHealth(agent, m_avatar1Health, m_avatar1MaxHP);
    }
    if (!m_avatar2Dead) // Don't draw avatar 2 if dead.
    {
        agent2->Draw();
        DrawText("2", agent2->GetPosition().x-MeasureText("2", 24)/2, agent2->GetPosition().y-12, 24, BLACK); // Avatar identifier.
        DrawAvatarHealth(agent2, m_avatar2Health, m_avatar2MaxHP);
    }

    // Draw Winning/Losing messages.
    if (!m_avatar1Dead && m_avatar2Dead)
        DrawText("Avatar 1 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && !m_avatar2Dead)
        DrawText("Avatar 2 Wins! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
    else if (m_avatar1Dead && m_avatar2Dead)
        DrawText("Both Lose! Press \'R\' to restart!", GetScreenWidth()/2, GetScreenHeight()/2, 32, BLACK);
}