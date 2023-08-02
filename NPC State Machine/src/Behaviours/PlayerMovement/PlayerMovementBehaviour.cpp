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
        if (IsKeyPressed(KEY_LEFT))
        {
            Node *point = FindLastNodeInDirection(agent, MOVE_DIRECTIONS::LEFT);
            if (point)
                agent->GoToNode(point);
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            Node *point = FindLastNodeInDirection(agent, MOVE_DIRECTIONS::RIGHT);
            if (point)
                agent->GoToNode(point);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            Node *point = FindLastNodeInDirection(agent, MOVE_DIRECTIONS::DOWN);
            if (point)
                agent->GoToNode(point);
        }
        if (IsKeyPressed(KEY_UP))
        {
            Node *point = FindLastNodeInDirection(agent, MOVE_DIRECTIONS::UP);
            if (point)
                agent->GoToNode(point);
        }

        Node *currentNode = agent->GetNodeMap()->GetClosestNode(agent->GetPosition());
        if (currentNode)
        {
            if (currentNode->type == NodeType::TELEPORT && agent->GetCurrentNode() == currentNode && !agent->PathComplete())
            {
                glm::vec2 pos = agent->GetPosition();
                Node *nextNode = nullptr;
                if (currentNode == agent->GetNodeMap()->GetClosestNode({ agent->GetNodeMap()->GetWidth()-1, pos.y }))
                {
                    pos.x = 0;
                    nextNode = agent->GetNodeMap()->GetClosestNode({ 1, pos.y });
                }
                if (currentNode == agent->GetNodeMap()->GetClosestNode({ 0, pos.y }))
                {
                    pos.x = agent->GetNodeMap()->GetWidth()-1;
                    nextNode = agent->GetNodeMap()->GetClosestNode({ agent->GetNodeMap()->GetWidth()-1, pos.y });
                }
                agent->SetPosition(pos);
                if (nextNode)
                {
                    agent->GoToNode(nextNode);
                }
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
                    point.x -= 1;
                }
            } break;
            case MOVE_DIRECTIONS::RIGHT:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.x += 1;
                }
            } break;
            case MOVE_DIRECTIONS::DOWN:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.y += 1;
                }
            } break;
            case MOVE_DIRECTIONS::UP:
            {
                glm::vec2 point = start;
                while (agent->GetNodeMap()->GetClosestNode(point) != nullptr)
                {
                    foundPoint = agent->GetNodeMap()->GetClosestNode(point);
                    point.y -= 1;
                }
            } break;
            default: break;
        }
        return foundPoint;
    }

}
