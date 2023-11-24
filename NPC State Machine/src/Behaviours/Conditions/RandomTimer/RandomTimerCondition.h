#pragma once

#include "Behaviours/Conditions/Condition.h"

namespace AIForGames
{
	class RandomTimerCondition : public Condition
	{
	public:
		RandomTimerCondition(float min, float max) : m_minTime(min), m_maxTime(max) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		float m_clock = 0.0f;
		float m_time;

		float m_minTime = 0.0f;
		float m_maxTime = 0.0f;

		bool m_pickedTime = false;

	};

}
