#include "Game.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"

#include "Utility/RenderText.h"

#include "Globals.h"

using namespace AIForGames;

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
    // TODO: Implement Stage Builder using "stage_parts" sprite sheet, 
    // in the original arcade game every level was the same stage but I eventually want stages to be easily created and customizable.
    std::vector<std::string> asciiMap;
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");
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
    asciiMap.push_back("0000001001111111111001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("0000001001000000001001000000");
    asciiMap.push_back("1111111111000000001111111111");
    asciiMap.push_back("0000001001000000001001000000");
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
    asciiMap.push_back("0000000000000000000000000000");
    asciiMap.push_back("0000000000000000000000000000");

    m_nodeMap->SetCellSize(8); // Tiles are 8x8
    m_nodeMap->Initialize(asciiMap);

    m_player = std::make_unique<Player>(m_nodeMap, m_nodeMap->GetNode(14, 26));

    m_blinky = std::make_unique<Blinky>(m_nodeMap, m_nodeMap->GetRandomNode());
    m_pinky = std::make_unique<Pinky>(m_nodeMap, m_nodeMap->GetRandomNode());
    m_inky = std::make_unique<Inky>(m_nodeMap, m_nodeMap->GetRandomNode());
    m_clyde = std::make_unique<Clyde>(m_nodeMap, m_nodeMap->GetRandomNode());

    m_resourceManager->AddTexture("TEX_STAGE", "./res/sprites/stage/stage.png"); // TODO: Draw stage from parts rather than from a full image.

    m_resourceManager->AddTexture("TEX_LIFECOUNTER", "./res/sprites/ui/lives.png");
    m_resourceManager->AddTexture("TEX_ITEMCOUNTER", "./res/sprites/ui/items.png");

    m_resourceManager->AddTexture("TEX_PACMAN", "./res/sprites/pacman.png");
    m_resourceManager->AddTexture("TEX_BLINKY", "./res/sprites/blinky.png");
    m_resourceManager->AddTexture("TEX_PINKY", "./res/sprites/pinky.png");
    m_resourceManager->AddTexture("TEX_INKY", "./res/sprites/inky.png");
    m_resourceManager->AddTexture("TEX_CLYDE", "./res/sprites/clyde.png");

    m_resourceManager->AddSound("SND_START", "./res/sfx/game_start.ogg");
    PlaySound(m_resourceManager->GetSound("SND_START"));
}

void Game::Cleanup()
{
    m_resourceManager->Cleanup();
}

float flashTimer = 0;
bool show_oneup = false;
void Game::Update(float deltaTime)
{
    if (flashTimer >= deltaTime * 15)
    {
        show_oneup = !show_oneup;
        flashTimer = 0;
    }
    flashTimer += deltaTime;

    if (!m_gameStart)
    {
        IntroSequence(deltaTime);
        return;
    }

    m_player->Update(deltaTime);

    m_blinky->Update(deltaTime);
    m_pinky->Update(deltaTime);
    m_inky->Update(deltaTime);
    m_clyde->Update(deltaTime);
}

void Game::Draw()
{
    // Draw Debug
    //m_nodeMap->Draw();

    // Draw Game
    if (m_introOne)
    {
        // TODO: Sprite Animation.
        m_player->Draw(m_resourceManager->GetTexture("TEX_PACMAN"));

        m_clyde->Draw(m_resourceManager->GetTexture("TEX_CLYDE"));
        m_inky->Draw(m_resourceManager->GetTexture("TEX_INKY"));
        m_pinky->Draw(m_resourceManager->GetTexture("TEX_PINKY"));
        m_blinky->Draw(m_resourceManager->GetTexture("TEX_BLINKY"));
    }

    DrawTextureEx(m_resourceManager->GetTexture("TEX_STAGE"), {0, 24}, 0, 1, WHITE); // Draw stage.

    // Draw UI
    const Color uiWhite = { 224, 221, 255, 255 };
    const Color uiBlue = { 0, 255, 255, 255 };
    const Color uiYellow = { 255, 255, 0, 255 };

    Utility::DrawString(Globals::g_fontTex, { 18, 0 }, "HIGH SCORE", uiWhite);

    if (show_oneup)
        Utility::DrawString(Globals::g_fontTex, { 5, 0 }, "1UP", uiWhite);

    std::stringstream scoreStream;
    scoreStream << std::setw(2) << std::setfill('0') << m_playerScore;
    Utility::DrawString(Globals::g_fontTex, { 6, 1 }, scoreStream.str(), uiWhite);

    scoreStream.clear(); // Clear out error bits
    scoreStream.str(std::string()); // Empty stream

    scoreStream << std::setw(2) << std::setfill('0') << m_highScore;
    Utility::DrawString(Globals::g_fontTex, { 16, 1 }, scoreStream.str(), uiWhite);

    if (m_playerLives > 0)
    {
        Texture &livesCounterTex = m_resourceManager->GetTexture("TEX_LIFECOUNTER");
        DrawTexturePro(livesCounterTex,
            { 0, (5 - (float)m_playerLives) * 16, (float)livesCounterTex.width, (float)livesCounterTex.height / 5 },
            { 2 * 8, 34 * 8, (float)livesCounterTex.width, (float)livesCounterTex.height / 5 },
            { 0, 0 },
            0, WHITE);
    }

    Texture &itemsCounterTex = m_resourceManager->GetTexture("TEX_ITEMCOUNTER");
    DrawTexturePro(itemsCounterTex,
        { 0, ((float)m_gameLevel) * 16, (float)itemsCounterTex.width, (float)itemsCounterTex.height / 19 },
        { 26 * 8, 34 * 8, (float)itemsCounterTex.width, (float)itemsCounterTex.height / 19 },
        { (float)itemsCounterTex.width, 0 },
        0, WHITE);

    // Draw Intro
    if (!m_introOne)
    {
        Utility::DrawString(Globals::g_fontTex, { 18, 14 }, "PLAYER ONE", uiBlue);
        Utility::DrawString(Globals::g_fontTex, { 16, 20 }, "READY!", uiYellow);
    }
    else if (m_introOne && !m_introTwo)
    {
        Utility::DrawString(Globals::g_fontTex, { 16, 20 }, "READY!", uiYellow);
    }
    else if (m_introOne && m_introTwo)
    {

    }
}

float introClock = 0;
void Game::IntroSequence(float deltaTime)
{
    // Timings
    if (introClock >= deltaTime * (3 * 60))
        m_introOne = true;
    if (introClock >= deltaTime * (4 * 60))
        m_introTwo = true;
    introClock += deltaTime;

    // Update
    if (!m_introOne)
    {
        m_playerLives = 5;
    }
    else if (m_introOne && !m_introTwo)
    {
        m_playerLives = 4;
    }
    else if (m_introOne && m_introTwo)
    {
        m_gameStart = true;
    }
}
