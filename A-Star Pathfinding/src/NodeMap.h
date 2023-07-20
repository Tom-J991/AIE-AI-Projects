#pragma once

#include "Pathfinding.h"

#include <string>
#include <functional>

namespace AIForGames
{
	class NodeMap
	{
	public:
		void Initialize(std::vector<std::string> asciiMap);

		void Draw();

		Node *GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
		Node *GetClosestNode(glm::vec2 worldPos);

		static std::vector<Node*> AStarSearch(Node *startNode, Node *endNode, std::function<float(Node*, Node*)> heuristicFunc);
		static float Heuristic(Node *startNode, Node *endNode);

	public:
		float cellSize = 8;

	private:
		int m_width, m_height;

		Node **m_nodes;

	};

}
