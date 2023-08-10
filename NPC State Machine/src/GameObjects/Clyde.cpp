#include "Clyde.h"

#include <raylib.h>

Clyde::Clyde(NodeMap *nodeMap, Node *startNode)
	: Ghost(nodeMap, startNode)
{
	m_agent->SetColor(ORANGE);
}
Clyde::~Clyde()
{ 
}

void Clyde::Update(float deltaTime)
{
	m_agent->Update(deltaTime);
}

void Clyde::Draw(Texture &texture)
{
	DrawPath(m_agent->GetPath(), ORANGE);

	DrawTexturePro(texture,
		{ 0 * 16, 0 * 16, 16, 16 },
		{ m_agent->GetPosition().x, m_agent->GetPosition().y, 16, 16 },
		{ 8, 8 },
		0, WHITE);
}
