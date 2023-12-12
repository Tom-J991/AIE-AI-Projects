#include "ABDecision.h"

namespace AIForGames
{
	ABDescision::ABDescision(Decision *decisionA, Decision *decisionB, Condition *condition)
	{
		m_decisionA = decisionA;
		m_decisionB = decisionB;
		m_condition = condition;
	}
	ABDescision::~ABDescision()
	{
		if (m_decisionA)
			delete m_decisionA;
		if (m_decisionB)
			delete m_decisionB;
		if (m_condition)
			delete m_condition;

		m_decisionA = nullptr;
		m_decisionB = nullptr;
		m_condition = nullptr;
	}

	void ABDescision::MakeDecision(Agent *agent, float deltaTime)
	{
		if (m_decisionA == nullptr || m_decisionB == nullptr || m_condition == nullptr)
			return;

		if (m_condition->IsTrue(agent, deltaTime))
			m_decisionA->MakeDecision(agent, deltaTime);
		else
			m_decisionB->MakeDecision(agent, deltaTime);
	}
}
