#pragma once

#include "Decisions/Decision.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	class AttackBehaviour : public Decision
	{
	public:
		AttackBehaviour(int *enemyHealth, bool *attackFlag);
		virtual ~AttackBehaviour();

		virtual void MakeDecision(Agent *agent, float deltaTime) override;

	private:
		int *m_enemyHealth;
		int m_damage = 0;
		
		float m_attackClock = 0.0f;
		float m_attackTime = 1.0f;

		bool m_doDamage = false;

		bool *m_avatarAttackFlag;

	};

}
