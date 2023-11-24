#include "FleeBehaviour.h"

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	void FleeBehaviour::Enter(Agent *agent)
	{
		m_storedSpeed = agent->GetSpeed();
		agent->SetSpeed(m_storedSpeed * 2);

		agent->SetColor(YELLOW);
		agent->Reset();
	}

	void FleeBehaviour::Update(Agent *agent, float deltaTime)
	{
		if (agent->PathComplete())
		{
			Node *destination = agent->GetNodeMap()->GetRandomNode();
			agent->GoToNode(destination);
		}
	}

	void FleeBehaviour::Exit(Agent *agent)
	{
		agent->SetSpeed(m_storedSpeed);
	}

	float FleeBehaviour::Evaluate(Agent *agent)
	{
		return 0.0f;
	}

}
