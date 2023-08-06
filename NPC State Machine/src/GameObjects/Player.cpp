#include "Player.h"

#include <raylib.h>

#include "Behaviours/PlayerMovement/PlayerMovementBehaviour.h"

Player::Player(NodeMap *nodeMap, Node *startNode)
{
    m_agent = new Agent(nodeMap, new PlayerMovementBehaviour());
    m_agent->SetNode(startNode);
    m_agent->SetPosition({ m_agent->GetPosition().x - 4.0f, m_agent->GetPosition().y });
    m_agent->SetColor(YELLOW);
    m_agent->SetSize(8);
    m_agent->SetSpeed(80);  // Pac-Man moves about 10 tiles per second by default, (tiles are 8x8, so 10*8 = 80).
                            // Pac-Man's speed is different depending on a few factors, so on the first level he should be moving at 80% of his speed so about 8 tiles per second.
}
Player::~Player()
{ 
    delete m_agent;
}

void Player::Update(float deltaTime)
{
    m_agent->Update(deltaTime);
}

void Player::Draw(Texture &texture)
{
    DrawPath(m_agent->GetPath(), YELLOW);

    DrawTexturePro(texture,
        { 1 * 16, 0 * 16, 16, 16 },
        { m_agent->GetPosition().x, m_agent->GetPosition().y, 16, 16 },
        { 8, 8 },
        0, WHITE);
}
