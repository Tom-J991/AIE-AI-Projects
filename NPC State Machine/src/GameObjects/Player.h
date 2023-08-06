#pragma once

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

using namespace AIForGames;

struct Texture;

class Player
{
public:
	Player(NodeMap *nodeMap, Node *startNode);
	~Player();

	void Update(float deltaTime);
	void Draw(Texture &texture);

private:
	Agent *m_agent;

};
