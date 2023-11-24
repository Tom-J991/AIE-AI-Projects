#pragma once

#include "Behaviours/Conditions/Condition.h"

namespace AIForGames
{
	class DistanceCondition : public Condition
	{
	public:
		DistanceCondition(float distance, bool lessThan = true) : m_distance(distance), m_lessThan(lessThan) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		float m_distance;
		bool m_lessThan = true;

	};

}
