#include "TimerCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool TimerCondition::IsTrue(Agent *agent, float deltaTime)
	{
		m_clock += deltaTime;

		if (m_clock >= m_time)
		{
			m_clock = 0.0f;
			return true;
		}

		return false;
	}

}
