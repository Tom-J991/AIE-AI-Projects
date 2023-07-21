#include "Agent.h"

#include "Pathfinding.h"
#include "NodeMap.h"
#include "Behaviours/Behaviour.h"

#include <raylib.h>

namespace AIForGames
{
	Agent::Agent(NodeMap *nodeMap, Behaviour *behaviour)
		: m_current(behaviour)
		, m_nodeMap(nodeMap)
		, m_color({ 255, 255, 0, 255 })
	{
		m_pathAgent.SetNodeMap(m_nodeMap);
		m_current->Enter(this);
		m_target = nullptr;
	}

	Agent::~Agent()
	{
		m_current->Exit(this);
		delete m_current;
	}

	void Agent::Update(float deltaTime)
	{
		if (m_current)
			m_current->Update(this, deltaTime);
		m_pathAgent.Update(deltaTime);
	}

	void Agent::Draw()
	{
		DrawCircle((int)m_pathAgent.GetPosition().x, (int)m_pathAgent.GetPosition().y, m_size, m_color);
	}

	void Agent::Reset()
	{
		m_pathAgent.GetPath().clear();
	}

	void Agent::GoToNode(Node *node)
	{
		m_pathAgent.GoToNode(node);
	}

	void Agent::GoToPoint(glm::vec2 point)
	{
		Node *end = m_nodeMap->GetClosestNode(point);
		m_pathAgent.GoToNode(end);
	}

}
