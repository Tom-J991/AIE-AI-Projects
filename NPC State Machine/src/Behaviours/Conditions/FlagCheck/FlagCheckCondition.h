#pragma once

#include "Behaviours/Conditions/Condition.h"

namespace AIForGames
{
	class FlagCheckCondition : public Condition
	{
	public:
		FlagCheckCondition(bool *flag, bool condition) : m_flagCheck(flag), m_condition(condition) { }

		virtual bool IsTrue(Agent *agent, float deltaTime) override;

	private:
		bool *m_flagCheck;
		bool m_condition;

	};

}
