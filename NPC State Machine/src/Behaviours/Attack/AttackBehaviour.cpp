#include "AttackBehaviour.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

#include "raylib.h"

namespace AIForGames
{
	void AttackBehaviour::Enter(Agent *agent)
	{
		m_damage = GetRandomValue(0, 2);

		agent->SetColor(RED);
		agent->Reset();
	}

	void AttackBehaviour::Update(Agent *agent, float deltaTime)
	{
		Agent *target = agent->GetTarget();

		if (m_attackClock >= m_attackTime)
		{
			*m_enemyHealth -= m_damage;
			m_attackClock = 0.0f;
		}

		m_attackClock += deltaTime;
	}

	void AttackBehaviour::Exit(Agent *agent)
	{

	}

	float AttackBehaviour::Evaluate(Agent *agent)
	{
		return 0.0f;
	}

}
