#pragma once

#include "PathFinding.h"

#include <raylib.h>

namespace AIForGames
{
	class NodeMap;

	class PathAgent
	{
	public:
		void GoToNode(Node *node);

		void Update(float deltaTime);
		void Draw();

		void SetNodeMap(NodeMap *nodeMap) { m_nodeMap = nodeMap; }

		void SetNode(Node *node) { m_currentNode = node; m_position = node->position; }

		float GetSpeed() const { return m_speed; }
		void SetSpeed(float speed) { m_speed = speed; }

		std::vector<Node*> &GetPath() { return m_path; }

		glm::vec2 GetPosition() const { return m_position; }
		void SetPosition(glm::vec2 position) { m_position = position; }

		Node *GetCurrentNode() const { return m_currentNode; }

	private:
		std::vector<Node*> m_path;

		int m_currentIndex;
		Node *m_currentNode;

		NodeMap *m_nodeMap;

		glm::vec2 m_position;
		float m_speed = 88;

	};

}
