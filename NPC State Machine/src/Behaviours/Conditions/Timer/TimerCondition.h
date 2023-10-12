#pragma once

#include "Behaviours/Conditions/Condition.h"

namespace AIForGames
{
	class TimerCondition : public Condition
	{
	public:
		TimerCondition(float t) : m_time(t) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		float m_clock = 0.0f;
		float m_time;

	};

}
