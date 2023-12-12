#pragma once

#include "Decisions/Conditions/Condition.h"

namespace AIForGames
{
	class HealthCondition : public Condition
	{
	public:
		HealthCondition(int *avatarHealth, int value, bool greaterThan = false) : m_avatarHealth(avatarHealth), m_value(value), m_greaterThan(greaterThan) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		int *m_avatarHealth;
		int m_value = 0.0f;

		bool m_greaterThan = false;

	};

}
