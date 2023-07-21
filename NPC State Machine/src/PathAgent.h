#pragma once

#include "PathFinding.h"

#include <raylib.h>

namespace AIForGames
{
	class PathAgent
	{
	public:
		void GoToNode(Node *node);

		void Update(float deltaTime);
		void Draw();

		void SetNode(Node *node) { m_currentNode = node; m_position = node->position; }
		void SetSpeed(float speed) { m_speed = speed; }

		std::vector<Node*> &GetPath() { return m_path; }

		glm::vec2 GetPosition() const { return m_position; }

	private:
		std::vector<Node*> m_path;

		int m_currentIndex;
		Node *m_currentNode;

		glm::vec2 m_position;
		float m_speed = 256;

	};

}
