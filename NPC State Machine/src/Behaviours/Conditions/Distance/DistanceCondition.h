#pragma once

#include "Behaviours/Conditions/Condition.h"

namespace AIForGames
{
	class DistanceCondition : public Condition
	{
	public:
		DistanceCondition(float d, bool lt) : m_distance(d), m_lessThan(lt) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		float m_distance;
		bool m_lessThan;

	};

}
