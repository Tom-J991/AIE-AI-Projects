#include "WanderBehaviour.h"

#include "Pathfinding.h"
#include "NodeMap.h"
#include "Agent.h"

namespace AIForGames
{
	void WanderBehaviour::Enter(Agent *agent)
	{
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
		return 0.0f;
	}

}
