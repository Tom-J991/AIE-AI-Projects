#include "RandomTimerCondition.h"

#include "Pathfinding/Agent.h"

#include <raylib.h>
#include <glm/glm.hpp>

namespace AIForGames
{
	bool RandomTimerCondition::IsTrue(Agent *agent, float deltaTime)
	{
		if (m_pickedTime == false)
		{
			m_time = GetRandomValue(m_minTime, m_maxTime);

			m_pickedTime = true;
		}

		m_clock += deltaTime;

		if (m_clock >= m_time)
		{
			m_clock = 0.0f;

			m_pickedTime = false;
			return true;
		}

		return false;
	}

}
