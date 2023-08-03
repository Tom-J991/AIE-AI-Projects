#include "PathAgent.h"

#include "NodeMap.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	void PathAgent::GoToNode(Node *node)
	{
		m_path = NodeMap::AStarSearch(m_currentNode, node, NodeMap::Heuristic); // Get path from pathfinding algorithm
		//m_path = m_nodeMap->SmoothPath(m_path); // Unused
		m_currentIndex = 0;
	}

	void PathAgent::Update(float deltaTime)
	{
		if (m_path.empty()) // If no path, no need to move.
			return;

		// Moves along path.
		glm::vec2 dir = glm::normalize((m_currentNode->position - m_position));
		float dist = glm::distance(m_position, m_currentNode->position);
		dist -= m_speed * deltaTime;
		if (dist >= 0)
		{
			m_position += dir * m_speed * deltaTime;
		}
		else
		{
			m_currentIndex++;

			if (m_currentIndex >= m_path.size())
			{
				m_position = m_currentNode->position;
				m_path.clear();
			}
			else
			{
				Node *oldNode = m_currentNode;
				m_currentNode = m_path[m_currentIndex];
				glm::vec2 newDir = glm::normalize((m_currentNode->position - oldNode->position));
				m_position = oldNode->position - dist * newDir;
			}
		}
	}

	void PathAgent::Draw()
	{
		
	}

}
