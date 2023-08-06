#include "Pinky.h"

#include <raylib.h>

Pinky::Pinky(NodeMap *nodeMap, Node *startNode)
	: Ghost(nodeMap, startNode)
{
	m_agent->SetColor(PINK);
}
Pinky::~Pinky()
{ 
	Ghost::~Ghost();
}

void Pinky::Update(float deltaTime)
{
	m_agent->Update(deltaTime);
}

void Pinky::Draw(Texture &texture)
{
	DrawPath(m_agent->GetPath(), PINK);

	DrawTexturePro(texture,
		{ 0 * 16, 0 * 16, 16, 16 },
		{ m_agent->GetPosition().x, m_agent->GetPosition().y, 16, 16 },
		{ 8, 8 },
		0, WHITE);
}
