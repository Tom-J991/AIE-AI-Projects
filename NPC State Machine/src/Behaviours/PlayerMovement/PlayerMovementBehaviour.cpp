#include "PlayerMovementBehaviour.h"

#include "Pathfinding.h"
#include "Agent.h"
#include "NodeMap.h"

#include <raylib.h>

#include <iostream>

namespace AIForGames
{
    void PlayerMovementBehaviour::Enter(Agent *agent)
    {
    }

    void PlayerMovementBehaviour::Update(Agent *agent, float deltaTime)
	{
        // Handle Input
        if (IsKeyPressed(KEY_LEFT))
        {
            m_inputDir = MOVE_DIRECTIONS::LEFT;
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            m_inputDir = MOVE_DIRECTIONS::RIGHT;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            m_inputDir = MOVE_DIRECTIONS::DOWN;
        }
        if (IsKeyPressed(KEY_UP))
        {
            m_inputDir = MOVE_DIRECTIONS::UP;
        }

        // Handle Movement
        if (m_inputDir != MOVE_DIRECTIONS::NONE)
        {
            if (IsPathBlockedInDirection(agent, m_inputDir)) // Avoids cancelling current movement if path is null.
                return;
            Node *point = FindLastNodeInDirection(agent, m_inputDir); // Finds last node by incrementing in direction until it is blocked.
            if (point)
            {
                agent->GoToNode(point);
                m_inputDir = MOVE_DIRECTIONS::NONE;
            }
        }
	}

    void PlayerMovementBehaviour::Exit(Agent *agent)
    {
    }

    float PlayerMovementBehaviour::Evaluate(Agent *agent)
    {
        return 0.0f;
    }

    Node *PlayerMovementBehaviour::FindLastNodeInDirection(Agent *agent, MOVE_DIRECTIONS direction)
    {
        // Increments in the specified direction until it is blocked by a wall, returns the last node in that direction.
        Node *foundPoint = nullptr;
        glm::vec2 start = agent->GetPosition();
        switch (direction)
        {
            case MOVE_DIRECTIONS::LEFT:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.x -= agent->GetNodeMap()->GetCellSize();
                }
            } break;
            case MOVE_DIRECTIONS::RIGHT:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.x += agent->GetNodeMap()->GetCellSize();
                }
            } break;
            case MOVE_DIRECTIONS::DOWN:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.y += agent->GetNodeMap()->GetCellSize();
                }
            } break;
            case MOVE_DIRECTIONS::UP:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.y -= agent->GetNodeMap()->GetCellSize();
                }
            } break;
            default: break;
        }
        return foundPoint;
    }

    bool PlayerMovementBehaviour::IsPathBlockedInDirection(Agent *agent, MOVE_DIRECTIONS direction)
    {
        // If point right next to the player is nullptr then the path is null.
        bool blocked = false;
        glm::vec2 start = agent->GetPosition();
        switch (direction)
        {
            case MOVE_DIRECTIONS::LEFT:
            {
                glm::vec2 point = start;
                point.x -= agent->GetNodeMap()->GetCellSize();
                if (agent->GetNodeMap()->GetClosestNode(point) == nullptr)
                    blocked = true;
            } break;
            case MOVE_DIRECTIONS::RIGHT:
            {
                glm::vec2 point = start;
                point.x += agent->GetNodeMap()->GetCellSize();
                if (agent->GetNodeMap()->GetClosestNode(point) == nullptr)
                    blocked = true;
            } break;
            case MOVE_DIRECTIONS::DOWN:
            {
                glm::vec2 point = start;
                point.y += agent->GetNodeMap()->GetCellSize();
                if (agent->GetNodeMap()->GetClosestNode(point) == nullptr)
                    blocked = true;
            } break;
            case MOVE_DIRECTIONS::UP:
            {
                glm::vec2 point = start;
                point.y -= agent->GetNodeMap()->GetCellSize();
                if (agent->GetNodeMap()->GetClosestNode(point) == nullptr)
                    blocked = true;
            } break;
            default: break;
        }
        return blocked;
    }

}
