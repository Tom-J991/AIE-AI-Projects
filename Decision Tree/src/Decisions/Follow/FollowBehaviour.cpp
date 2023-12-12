#include "FollowBehaviour.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	/*void FollowBehaviour::Enter(Agent *agent)
	{
		agent->SetColor(BLUE);
		agent->Reset();
	}*/

	FollowBehaviour::FollowBehaviour()
	{
	}
	FollowBehaviour::~FollowBehaviour()
	{
	}

	void FollowBehaviour::MakeDecision(Agent *agent, float deltaTime)
	{
		if (!CompareColor(agent->GetColor(), BLUE))
		{
			// Bad workaround that basically acts as an On Entry method like in the state machine, relies on the color of the agent.
			//agent->Reset();
		}
		agent->SetColor(BLUE);

		Agent *target = agent->GetTarget();

		// Get the last target's position, go to it.
		float dist = glm::distance(target->GetPosition(), lastTargetPosition);
		if (dist > agent->GetNodeMap()->GetCellSize())
		{
			lastTargetPosition = target->GetPosition();
			agent->GoToPoint(lastTargetPosition);
		}

	}

}
