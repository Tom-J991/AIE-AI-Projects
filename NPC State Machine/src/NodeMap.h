#pragma once

#include <glm/glm.hpp>

#include <string>
#include <functional>
#include <vector>

namespace AIForGames
{
	struct Node;

	class NodeMap
	{
	public:
		void Initialize(std::vector<std::string> asciiMap);

		void Draw();

		Node *GetNode(int x, int y) const { return m_nodes[x + m_width * y]; }
		Node *GetClosestNode(glm::vec2 worldPos) const;
		Node *GetRandomNode() const;

		float GetCellSize() const { return m_cellSize; }

		static std::vector<Node*> AStarSearch(Node *startNode, Node *endNode, std::function<float(Node*, Node*)> heuristicFunc);
		static float Heuristic(Node *startNode, Node *endNode);

	private:
		float m_cellSize = 64;
		int m_width, m_height;

		Node **m_nodes;

	};

}
