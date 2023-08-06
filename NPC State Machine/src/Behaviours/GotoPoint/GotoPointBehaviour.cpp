#include "GotoPointBehaviour.h"

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/Agent.h"

#include <raylib.h>

namespace AIForGames
{
    void GotoPointBehaviour::Enter(Agent *agent)
    {
    }

    void GotoPointBehaviour::Update(Agent *agent, float deltaTime)
	{
        if (IsMouseButtonPressed(0))
        {
            Vector2 mousePos = GetMousePosition();
            agent->GoToPoint(glm::vec2(mousePos.x, mousePos.y));
        }
	}

    void GotoPointBehaviour::Exit(Agent *agent)
    {
    }

    float GotoPointBehaviour::Evaluate(Agent *agent)
    {
        return 0.0f;
    }

}
