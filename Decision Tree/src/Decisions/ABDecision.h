#pragma once

#include "Decision.h"
#include "Conditions/Condition.h"

namespace AIForGames
{
	class ABDescision : public Decision
	{
	public:
		ABDescision(Decision *decisionA, Decision *decisionB, Condition *condition);
		virtual ~ABDescision();

		virtual void MakeDecision(Agent *agent, float deltaTime) override;

	private:
		Decision *m_decisionA = nullptr;
		Decision *m_decisionB = nullptr;

		Condition *m_condition = nullptr;

	};

}
