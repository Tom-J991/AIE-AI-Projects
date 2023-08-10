#pragma once

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

#include "Behaviours/FiniteStateMachine.h"

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
	FiniteStateMachine *m_stateMachine;

private:

};
