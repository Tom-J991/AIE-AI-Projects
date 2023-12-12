#include "Pathfinding.h"

#include <raylib.h>

namespace AIForGames
{
	void Node::ConnectTo(Node *other, float cost)
	{
		connections.push_back(Edge(other, cost));

	}

    void DrawPath(std::vector<Node*> path, Color lineColor)
    {
        for (int i = 1; i < path.size(); ++i)
        {
            Node *start = path[i-1];
            Node *end = path[i];
            DrawLine((int)start->position.x, (int)start->position.y, (int)end->position.x, (int)end->position.y, lineColor);
        }

    }

}
