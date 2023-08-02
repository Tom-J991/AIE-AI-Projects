#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Color;

namespace AIForGames
{
    enum class NodeType
    {
        NORMAL = 0,
        TELEPORT = 1,
        NODE_MAX
    };

    struct Node;

    struct Edge {
        Edge() { target = nullptr; cost = 0; }
        Edge(Node *_target, float _cost) : target(_target), cost(_cost) { }

        Node* target;
        float cost;

    };

    struct Node {
        Node() { }
        Node(float x, float y) { position = glm::vec2(x, y); }

        glm::vec2 position;
        float gScore;
        float hScore;
        float fScore;
        Node *previous;
        std::vector<Edge> connections;
        NodeType type = NodeType::NORMAL;

        void ConnectTo(Node *other, float cost);

    };

    void DrawPath(std::vector<Node*> path, Color lineColor);

}
