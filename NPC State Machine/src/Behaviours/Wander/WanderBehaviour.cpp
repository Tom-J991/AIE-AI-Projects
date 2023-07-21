#include "WanderBehaviour.h"

#include "Pathfinding.h"
#include "NodeMap.h"
#include "Agent.h"

namespace AIForGames
{
	void WanderBehaviour::Enter(Agent *agent)
	{
		agent->SetColor({ 0, 255, 255, 255 });
		agent->Reset();
	}

	void WanderBehaviour::Update(Agent *agent, float deltaTime)
	{
		if (agent->PathComplete())
		{
			Node *destination = agent->GetNodeMap()->GetRandomNode();
			agent->GoToNode(destination);
		}
	}

	void WanderBehaviour::Exit(Agent *agent)
	{

	}

	float WanderBehaviour::Evaluate(Agent *agent)
	{
		Agent *target = agent->GetTarget();
		float dist = glm::distance(target->GetPosition(), agent->GetPosition());

		float eval = dist;
		if (eval < 0)
			eval = 0;

		return eval;
	}

}
