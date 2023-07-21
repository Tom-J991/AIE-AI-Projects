#pragma once

#include "Conditions/Condition.h"

namespace AIForGames
{
	class DistanceCondition : public Condition
	{
	public:
		DistanceCondition(float d, bool lt) : m_distance(d), m_lessThan(lt) { }

		virtual bool IsTrue(Agent *agent) override;

	private:
		float m_distance;
		bool m_lessThan;

	};

}
