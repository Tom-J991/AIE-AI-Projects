#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace AIForGames
{
    struct Node;

    struct Edge {
        Node* target;
        float cost;
    };

    struct Node {
        glm::vec2 position;
        std::vector<Edge> connections;
    };

}
