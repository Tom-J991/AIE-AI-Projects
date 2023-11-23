#include "AttackBehaviour.h"

#include <iostream>
#include <raylib.h>

#include "Globals.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

namespace AIForGames
{
	void AttackBehaviour::Enter(Agent *agent)
	{
		m_damage = GetRandomValue(0, 2);
		if (Globals::g_difficulty > 1)
		{
			int mult = Globals::g_difficulty-1;
			int newDmgMax = GetRandomValue(2, 2+mult);
			m_damage = GetRandomValue(0, newDmgMax);

			m_attackTime = GetRandomValue(1.0f, 1.0f+(0.5f*mult));
		}

		m_doDamage = true;

		agent->SetColor(RED);
		agent->Reset();
	}

	void AttackBehaviour::Update(Agent *agent, float deltaTime)
	{
		Agent *target = agent->GetTarget();

		if (m_attackClock >= m_attackTime * deltaTime && m_doDamage == true)
		{
			*m_enemyHealth -= m_damage;
			std::cout << "Damage Dealt: " << m_damage << std::endl;

			m_doDamage = false;
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
