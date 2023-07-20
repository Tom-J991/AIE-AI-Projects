#pragma once

#include "Pathfinding.h"

namespace AIForGames
{
	class PathAgent
	{
	public:
		void GoToNode(Node *node);

		void Update(float deltaTime);
		void Draw(float size);

		void SetNode(Node *node) { m_currentNode = node; m_position = node->position; }
		void SetSpeed(float speed) { m_speed = speed; }

	public:
		std::vector<Node*> path;

	private:
		int m_currentIndex;
		Node *m_currentNode;

		glm::vec2 m_position;
		float m_speed;

	};

}
