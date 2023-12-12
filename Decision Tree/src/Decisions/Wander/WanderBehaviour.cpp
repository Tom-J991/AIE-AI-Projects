#include "WanderBehaviour.h"

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	WanderBehaviour::WanderBehaviour()
	{
	}
	WanderBehaviour::~WanderBehaviour()
	{
	}

	/*void WanderBehaviour::Enter(Agent *agent)
	{
		agent->SetColor(GREEN);
		agent->Reset();
	}*/

	void WanderBehaviour::MakeDecision(Agent *agent, float deltaTime)
	{
		agent->SetColor(GREEN);

		if (agent->PathComplete())
		{
			// Find random spot, go to it.
			Node *destination = agent->GetNodeMap()->GetRandomNode();
			agent->GoToNode(destination);
		}
	}

}
