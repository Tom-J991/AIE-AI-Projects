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

		bool IsVisibleFrom(Node *start, Node *end);

		std::vector<Node*> SmoothPath(std::vector<Node*> path);

		Node *GetNode(int x, int y) const { return m_nodes[x + m_width * y]; }
		Node *GetClosestNode(glm::vec2 worldPos) const;
		Node *GetRandomNode() const;

		void SetCellSize(float cellSize) { m_cellSize = cellSize; }
		float GetCellSize() const { return m_cellSize; }

		static std::vector<Node*> AStarSearch(Node *startNode, Node *endNode, std::function<float(Node*, Node*)> heuristicFunc);
		static float Heuristic(Node *startNode, Node *endNode);

	private:
		float m_cellSize = 64;
		int m_width, m_height;

		Node **m_nodes;

	};

}
