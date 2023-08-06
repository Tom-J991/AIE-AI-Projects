#include "SelectorBehaviour.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	void SelectorBehaviour::Enter(Agent *agent)
	{
	}
	void SelectorBehaviour::Update(Agent *agent, float deltaTime)
	{
		if (glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition()) < agent->GetNodeMap()->GetCellSize() * 5)
		{
			SetBehaviour(m_b1, agent);
			agent->SetColor({ 255, 0, 0, 255 });
		}
		else
		{
			SetBehaviour(m_b2, agent);
			agent->SetColor({ 0, 255, 255, 255 });
		}
		m_selected->Update(agent, deltaTime);
	}

	void SelectorBehaviour::Exit(Agent *agent)
	{
	}

	float SelectorBehaviour::Evaluate(Agent *agent)
	{
		return 0.0f;
	}

	void SelectorBehaviour::SetBehaviour(Behaviour *b, Agent *agent)
	{
		if (m_selected != b)
		{
			m_selected = b;
			agent->Reset();
		}

	}

}
