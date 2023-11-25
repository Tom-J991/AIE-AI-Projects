#include "IdleBehaviour.h"

#include "Pathfinding/Pathfinding.h"
#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	void IdleBehaviour::Enter(Agent *agent)
	{
		agent->SetColor(PINK);
		agent->Reset();
	}

	void IdleBehaviour::Update(Agent *agent, float deltaTime)
	{

	}

	void IdleBehaviour::Exit(Agent *agent)
	{

	}

	float IdleBehaviour::Evaluate(Agent *agent)
	{
		return 0.0f;
	}

}
