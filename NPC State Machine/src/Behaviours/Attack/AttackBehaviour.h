#pragma once

#include "Behaviours/Behaviour.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	class AttackBehaviour : public Behaviour
	{
	public:
		AttackBehaviour(int *enemyHealth) : m_enemyHealth(enemyHealth)
		{ }

		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

	private:
		int *m_enemyHealth;
		int m_damage = 0;
		
		float m_attackClock = 0.0f;
		float m_attackTime = 1.0f;

	};

}
