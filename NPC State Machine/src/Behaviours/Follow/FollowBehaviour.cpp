#include "FollowBehaviour.h"

#include "NodeMap.h"
#include "Agent.h"

namespace AIForGames
{
	void FollowBehaviour::Enter(Agent *agent)
	{
		agent->SetColor({ 255, 0, 0, 255 });
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
		Agent *target = agent->GetTarget();
		float dist = glm::distance(target->GetPosition(), agent->GetPosition());

		float eval = 10 * agent->GetNodeMap()->GetCellSize() - dist;
		if (eval < 0)
			eval = 0;

		return eval;
	}

}
