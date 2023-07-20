#pragma once

#include <raylib.h>

#include <glm/glm.hpp>
#include <vector>

namespace AIForGames
{
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
        Node *previous;
        std::vector<Edge> connections;

        void ConnectTo(Node *other, float cost);

    };

    void DrawPath(std::vector<Node*> path, Color lineColor);

}
