#include "Blinky.h"

#include <raylib.h>

Blinky::Blinky(NodeMap *nodeMap, Node *startNode)
	: Ghost(nodeMap, startNode)
{
	m_agent->SetColor(RED);
}
Blinky::~Blinky()
{ 
	Ghost::~Ghost();
}

void Blinky::Update(float deltaTime)
{
	m_agent->Update(deltaTime);
}

void Blinky::Draw(Texture &texture)
{
	DrawPath(m_agent->GetPath(), RED);

	DrawTexturePro(texture,
		{ 0 * 16, 0 * 16, 16, 16 },
		{ m_agent->GetPosition().x, m_agent->GetPosition().y, 16, 16 },
		{ 8, 8 },
		0, WHITE);
}
