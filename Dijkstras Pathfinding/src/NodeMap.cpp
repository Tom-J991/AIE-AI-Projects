#include "NodeMap.h"

#include <raylib.h>

#include <iostream>
#include <algorithm>

namespace AIForGames
{
	void NodeMap::Initialize(std::vector<std::string> asciiMap)
	{
		const char emptySquare = '0';

		m_height = asciiMap.size();
		m_width = asciiMap[0].size();

		m_nodes = new Node*[m_width * m_height];

		for (int y = 0; y < m_height; ++y)
		{
			std::string &line = asciiMap[y];
			if (line.size() != m_width)
				std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;

			for (int x = 0; x < m_width; ++x)
			{
				char tile = x < line.size() ? line[x] : emptySquare;
				m_nodes[x + m_width * y] = tile == emptySquare ? nullptr : new Node(((float)x + 0.5f) * cellSize, ((float)y + 0.5f) * cellSize);
			}
		}

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
						node->ConnectTo(nodeWest, 1); // TODO: weights
						nodeWest->ConnectTo(node, 1);
					}

					Node *nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
					if (nodeSouth)
					{
						node->ConnectTo(nodeSouth, 1); // TODO: weights
						nodeSouth->ConnectTo(node, 1);
					}
				}
			}
		}

	}

	Node *NodeMap::GetClosestNode(glm::vec2 worldPos)
	{
		int i = (int)(worldPos.x / cellSize);
		if (i < 0 || i > m_width) return nullptr;
		int j = (int)(worldPos.y / cellSize);
		if (j < 0 || j > m_height) return nullptr;
		return GetNode(i, j);
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
					DrawRectangle((int)(x * cellSize), (int)(y * cellSize), (int)cellSize - 1, (int)cellSize - 1, cellColor);
				}
				else
				{
					for (int i = 0; i < node->connections.size(); i++)
					{
						Node *other = node->connections[i].target;
						DrawLine((x + 0.5f) * cellSize, (y + 0.5f) * cellSize, (int)other->position.x, (int)other->position.y, lineColor);
					}
				}
			}
		}

	}

	std::vector<Node *> NodeMap::DijkstrasSearch(Node *startNode, Node *endNode)
	{
		if (startNode == nullptr || endNode == nullptr)
		{
			std::cout << "Error in Dijkstras Search. One or both parameters are nullptr!" << std::endl;
			return std::vector<Node*>();
		}

		if (startNode == endNode)
			return std::vector<Node*>();

		startNode->gScore = 0;
		startNode->previous = nullptr;

		std::vector<Node *> openList;
		std::vector<Node *> closedList;

		openList.push_back(startNode);

		while (!openList.empty())
		{
			std::sort(openList.begin(), openList.end(), [](Node *a, Node *b) { return (a->gScore < b->gScore); });

			Node *currentNode = openList.front();
			if (currentNode == endNode)
				break;
			openList.erase(openList.begin());
			closedList.push_back(currentNode);

			for (Edge &c : currentNode->connections)
			{
				if (std::find(closedList.begin(), closedList.end(), c.target) != closedList.end())
					continue;

				if (std::find(openList.begin(), openList.end(), c.target) == openList.end())
				{
					c.target->gScore = currentNode->gScore + c.cost;
					c.target->previous = currentNode;
					auto insert = openList.end();
					for (auto i = openList.begin(); i != openList.end(); ++i)
					{
						if (c.target->gScore < (*i)->gScore)
						{
							insert = i;
							break;
						}
					}
					openList.insert(insert, c.target);
				}
				else if (currentNode->gScore + c.cost < c.target->gScore)
				{
					c.target->gScore = currentNode->gScore + c.cost;
					c.target->previous = currentNode;
				}
			}
		}

		std::vector<Node *> path;
		Node *currentNode = endNode;

		while (currentNode != nullptr)
		{
			path.insert(path.begin(), currentNode);
			currentNode = currentNode->previous;
		}

		return path;
	}

}
