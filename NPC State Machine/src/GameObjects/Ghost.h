#pragma once

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

using namespace AIForGames;

struct Texture;

class Ghost
{
public:
	Ghost(NodeMap *nodeMap, Node *startNode);
	virtual ~Ghost();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Texture &texture) = 0;

protected:
	Agent *m_agent;

private:

};
