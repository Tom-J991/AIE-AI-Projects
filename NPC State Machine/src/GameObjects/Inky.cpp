#include "Inky.h"

#include <raylib.h>

Inky::Inky(NodeMap *nodeMap, Node *startNode)
	: Ghost(nodeMap, startNode)
{
	m_agent->SetColor(SKYBLUE);
}
Inky::~Inky()
{ 
}

void Inky::Update(float deltaTime)
{
	m_agent->Update(deltaTime);
}

void Inky::Draw(Texture &texture)
{
	DrawPath(m_agent->GetPath(), SKYBLUE);

	DrawTexturePro(texture,
		{ 0 * 16, 0 * 16, 16, 16 },
		{ m_agent->GetPosition().x, m_agent->GetPosition().y, 16, 16 },
		{ 8, 8 },
		0, WHITE);
}
