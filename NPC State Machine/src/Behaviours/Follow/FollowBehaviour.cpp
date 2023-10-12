#include "FollowBehaviour.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	void FollowBehaviour::Enter(Agent *agent)
	{
		agent->SetColor(RED);
		agent->Reset();
	}

	void FollowBehaviour::Update(Agent *agent, float deltaTime)
	{
		Agent *target = agent->GetTarget();

		float dist = glm::distance(target->GetPosition(), lastTargetPosition);
		if (dist > agent->GetNodeMap()->GetCellSize())
		{
			lastTargetPosition = target->GetPosition();
			agent->GoToPoint(lastTargetPosition);
		}

	}

	void FollowBehaviour::Exit(Agent *agent)
	{

	}

	float FollowBehaviour::Evaluate(Agent *agent)
	{
		return 0.0f;
	}

}
