#include "RandomTimerCondition.h"

#include "Pathfinding/Agent.h"

#include <raylib.h>
#include <glm/glm.hpp>

namespace AIForGames
{
	bool RandomTimerCondition::IsTrue(Agent *agent, float deltaTime)
	{
		// Pick a random time to count to.
		if (m_pickedTime == false)
		{
			m_time = GetRandomValue(m_minTime, m_maxTime);
			m_pickedTime = true; // Flag to only pick the time once.
		}

		m_clock += deltaTime; // Tick tock.
		if (m_clock >= m_time) // Return true if the random time is met.
		{
			m_clock = 0.0f; // Reset clock and flag.
			m_pickedTime = false;
			return true;
		}

		return false;
	}

}
