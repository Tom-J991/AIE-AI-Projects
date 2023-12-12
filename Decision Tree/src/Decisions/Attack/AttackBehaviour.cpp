#include "AttackBehaviour.h"

#include <iostream>
#include <raylib.h>

#include "Globals.h"

#include "Pathfinding/NodeMap.h"
#include "Pathfinding/Agent.h"

// Arrays to randomly access resources.
const char *attackSFX[9] = {
	"HIT1", "HIT2", "HIT3", "HIT4", "HIT5", "HIT6", "HIT7", "HIT8", "HIT9"
};
const char *missSFX[5] = {
	"MISS1", "MISS2", "MISS3", "MISS4", "MISS5"
};

namespace AIForGames
{
	/*void AttackBehaviour::Enter(Agent *agent)
	{
		m_damage = GetRandomValue(0, 2); // Randomize the damage between 0 and the max, 0 gives the avatar a chance to miss.
		if (Globals::g_difficulty > 1) // If difficulty is above 1, randomize the damage and attack time a bit based on the difficulty.
		{
			int mult = Globals::g_difficulty-1;
			int newDmgMax = GetRandomValue(2, 2+mult);
			m_damage = GetRandomValue(0, newDmgMax);

			m_attackTime = GetRandomValue(1.0f, 1.0f+(0.5f*mult));
		}

		m_doDamage = true; // Flag so the avatar can only attack once and not on every frame (because on update).
		*m_avatarAttackFlag = true; // Tell the variable in the game class that the avatar is attacking.

		agent->SetColor(RED);
		agent->Reset();
	}*/

	AttackBehaviour::AttackBehaviour(int *enemyHealth, bool *attackFlag)
		: m_enemyHealth(enemyHealth), m_avatarAttackFlag(attackFlag)
	{

	}
	AttackBehaviour::~AttackBehaviour()
	{

	}

	void AttackBehaviour::MakeDecision(Agent *agent, float deltaTime)
	{
		if (!CompareColor(agent->GetColor(), RED))
		{
			// Bad workaround that basically acts as an On Entry method like in the state machine, relies on the color of the agent.
			m_damage = GetRandomValue(0, 2); // Randomize the damage between 0 and the max, 0 gives the avatar a chance to miss.
			if (Globals::g_difficulty > 1) // If difficulty is above 1, randomize the damage and attack time a bit based on the difficulty.
			{
				int mult = Globals::g_difficulty - 1;
				int newDmgMax = GetRandomValue(2, 2 + mult);
				m_damage = GetRandomValue(0, newDmgMax);

				m_attackTime = GetRandomValue(1.0f, 1.0f + (0.5f * mult));
			}

			m_doDamage = true; // Flag so the avatar can only attack once and not on every frame (because on update).
			*m_avatarAttackFlag = true; // Tell the variable in the game class that the avatar is attacking.

			agent->Reset();
		}
		agent->SetColor(RED);

		Agent *target = agent->GetTarget();

		if (m_attackClock >= m_attackTime * deltaTime && m_doDamage == true) // Attack cooldown.
		{
			if (*m_enemyHealth <= 0) // Don't try to attack thin air.
			{
				m_doDamage = false;
				m_attackClock = 0.0f;
				return;
			}

			*m_enemyHealth -= m_damage; // Deal damage to the enemy.
			std::cout << "Damage Dealt: " << m_damage << std::endl;

			*m_avatarAttackFlag = false; // Tell the variable in the game class that the avatar is not attacking.

			int rng = GetRandomValue(0, 8); // Play a random sound from a list.
			if (m_damage > 0)
				PlaySound(Globals::g_resourceManager->GetSound(attackSFX[rng]));
			else
				PlaySound(Globals::g_resourceManager->GetSound(missSFX[rng % 5]));

			if (agent->PathComplete()) // Flee
			{
				// Find random spot, go to it.
				Node *destination = agent->GetNodeMap()->GetRandomNode();
				agent->GoToNode(destination);
			}

			m_doDamage = false; // Reset flags and clock.
			m_attackClock = 0.0f;
		}

		m_attackClock += deltaTime; // Tick tock.
	}

}
