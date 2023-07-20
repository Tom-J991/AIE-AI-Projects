#include "PathAgent.h"

#include "NodeMap.h"

namespace AIForGames
{
	void PathAgent::GoToNode(Node *node)
	{
		path = NodeMap::AStarSearch(m_currentNode, node, NodeMap::Heuristic);
		m_currentIndex = 0;
	}

	void PathAgent::Update(float deltaTime)
	{
		if (path.empty())
			return;

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

			if (m_currentIndex >= path.size())
			{
				m_position = m_currentNode->position;
				path.clear();
			}
			else
			{
				Node *oldNode = m_currentNode;
				m_currentNode = path[m_currentIndex];
				glm::vec2 newDir = glm::normalize((m_currentNode->position - oldNode->position));
				m_position = oldNode->position - dist * newDir;
			}
		}
	}

	void PathAgent::Draw(float size)
	{
		DrawCircle((int)m_position.x, (int)m_position.y, size, { 255, 255, 0, 255 });
	}

}
