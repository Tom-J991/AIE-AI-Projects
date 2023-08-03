#include "NodeMap.h"

#include "Pathfinding.h"

#include <raylib.h>

#include <iostream>
#include <algorithm>

namespace AIForGames
{
	void NodeMap::Initialize(std::vector<std::string> asciiMap)
	{
		// Create node map from ascii
		const char emptySquare = '0';
		const char normalSquare = '1';

		m_height = asciiMap.size();
		m_width = asciiMap[0].size();

		m_nodes = new Node*[m_width * m_height];

		for (int y = 0; y < m_height; ++y)
		{
			std::string &line = asciiMap[y];
			if (line.size() != m_width) // If a line is inconsistent with the rest.
				std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;

			for (int x = 0; x < m_width; ++x)
			{
				// Create nodes
				char tile = x < line.size() ? line[x] : emptySquare;
				if (tile == emptySquare)
					m_nodes[x + m_width * y] = nullptr;
				else
					m_nodes[x + m_width * y] = new Node(((float)x + 0.5f) * m_cellSize, ((float)y + 0.5f) * m_cellSize);
			}
		}

		// Connect nodes
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				Node *node = GetNode(x, y);
				if (node)
				{
					Node *nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);
					if (nodeWest)
					{
						node->ConnectTo(nodeWest, 1.f);
						nodeWest->ConnectTo(node, 1.f);
					}
					Node *nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
					if (nodeSouth)
					{
						node->ConnectTo(nodeSouth, 1.f);
						nodeSouth->ConnectTo(node, 1.f);
					}

					// Diagonals (Unused)
					/*Node *nodeSouthWest = (x == 0 || y == 0) ? nullptr : GetNode(x-1, y-1);
					if (nodeSouthWest)
					{
						node->ConnectTo(nodeSouthWest, 1.f * 1.414f);
						nodeSouthWest->ConnectTo(node, 1.f * 1.414f);
					}
					Node *nodeSouthEast = (x == m_width-1 || y == 0) ? nullptr : GetNode(x+1, y-1);
					if (nodeSouthEast)
					{
						node->ConnectTo(nodeSouthEast, 1.f * 1.414f);
						nodeSouthEast->ConnectTo(node, 1.f * 1.414f);
					}*/
				}
			}
		}

	}

	Node *NodeMap::GetClosestNode(glm::vec2 worldPos) const
	{
		int i = (int)(worldPos.x / m_cellSize);
		if (i < 0 || i > m_width) return nullptr;
		int j = (int)(worldPos.y / m_cellSize);
		if (j < 0 || j > m_height) return nullptr;
		return GetNode(i, j);
	}

	Node *NodeMap::GetRandomNode() const
	{
		Node *node = nullptr;
		while (node == nullptr)
		{
			int x = rand() % m_width;
			int y = rand() % m_height;
			node = GetNode(x, y);
		}
		return node;
	}

	void NodeMap::Draw()
	{
		Color cellColor ={ 255, 0, 0, 255 };
		Color lineColor ={ 128, 128, 128, 255 };

		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				Node *node = GetNode(x, y);
				if (!node)
				{
					// Draws walls, (Unused)
					//DrawRectangle((int)(x * m_cellSize), (int)(y * m_cellSize), (int)m_cellSize - 1, (int)m_cellSize - 1, cellColor);
				}
				else
				{
					// Draw Path
					for (int i = 0; i < node->connections.size(); i++)
					{
						Node *other = node->connections[i].target;
						DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize, (int)other->position.x, (int)other->position.y, lineColor);
					}
				}
			}
		}

	}

	bool NodeMap::IsVisibleFrom(Node *start, Node *end)
	{
		glm::vec2 delta = end->position - start->position;
		float distance = glm::distance(end->position, start->position);
		delta = delta * (m_cellSize/distance);

		// Increment along path until blocked, return false if it's blocked, return true if it's not.
		for (float cells = 1.0f; cells < distance/m_cellSize; cells += 1.0f)
		{
			glm::vec2 testPos = start->position + (delta * cells);
			if (GetClosestNode(testPos) == nullptr)
				return false;
		}

		return true;
	}

	std::vector<Node*> NodeMap::SmoothPath(std::vector<Node*> path)
	{
		if (path.size() == 0)
			return path;

		std::vector<Node*> smoothed;

		Node *start = path[0];
		smoothed.push_back(start);

		// Remove unnecessary nodes from path.
		for (int i = 0; i < path.size(); ++i)
		{
			while (i < path.size() - 1 && IsVisibleFrom(start, path[i + 1]))
				i++;
			start = path[i];
			smoothed.push_back(start);
		}

		return smoothed;
	}

	std::vector<Node*> NodeMap::AStarSearch(Node *startNode, Node *endNode, std::function<float(Node*, Node*)> heuristicFunc)
	{ 
		// A* Implementation

		if (startNode == nullptr || endNode == nullptr) // Needs a start and an end.
		{
			std::cout << "Error in Dijkstras Search. One or both parameters are nullptr!" << std::endl;
			return std::vector<Node*>(); // Return empty path.
		}

		if (startNode == endNode) // If start is also the end then there is no path.
			return std::vector<Node*>();

		// Init
		startNode->gScore = 0;
		startNode->previous = nullptr;

		std::vector<Node *> openList; // Visiting list
		std::vector<Node *> closedList; // Visited list

		openList.push_back(startNode);

		while (!openList.empty())
		{
			// Resort list by final score to guarantee the shortest path.
			std::sort(openList.begin(), openList.end(), [](Node *a, Node *b) { return (a->fScore < b->fScore); });

			Node *currentNode = openList.front();

			// Exit loop early if end node is found.
			if (currentNode == endNode)
				break;

			openList.erase(openList.begin());
			closedList.push_back(currentNode);

			for (Edge &c : currentNode->connections)
			{
				if (std::find(closedList.begin(), closedList.end(), c.target) != closedList.end())
					continue;

				float gScore = currentNode->gScore + c.cost;
				float hScore = heuristicFunc(c.target, endNode);
				float fScore = gScore + hScore;

				// Calculate the score and update parent if node hasn't been visited yet.
				// Add it to the open list to be compared.
				if (std::find(openList.begin(), openList.end(), c.target) == openList.end())
				{
					c.target->gScore = gScore;
					c.target->hScore = hScore;
					c.target->fScore = fScore;
					c.target->previous = currentNode;
					auto insert = openList.end();
					for (auto i = openList.begin(); i != openList.end(); ++i)
					{
						if (c.target->fScore < (*i)->fScore)
						{
							insert = i;
							break;
						}
					}
					openList.insert(insert, c.target);
				}
				else if (fScore < c.target->fScore) // Node is already in the open list with a valid score, compare scores to find the shorter path.
				{
					c.target->gScore = gScore;
					c.target->hScore = hScore;
					c.target->fScore = fScore;
					c.target->previous = currentNode;
				}
			}
		}

		// Create path.
		std::vector<Node *> path;
		Node *currentNode = endNode;

		while (currentNode != nullptr)
		{
			path.insert(path.begin(), currentNode);
			currentNode = currentNode->previous;
		}

		return path;
	}

	float NodeMap::Heuristic(Node *startNode, Node *endNode)
	{
		// Default heuristic function.
		// Manhattan Distance
		float xDiff = abs(startNode->position.x - endNode->position.x);
		float yDiff = abs(startNode->position.y - endNode->position.y);
		return (xDiff + yDiff);
	}

}
