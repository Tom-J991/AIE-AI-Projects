#include "TimerCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool TimerCondition::IsTrue(Agent *agent, float deltaTime)
	{
		m_clock += deltaTime; // Tick tock.

		if (m_clock >= m_time) // Returns true if the given time is met.
		{
			m_clock = 0.0f; // Reset clock.
			return true;
		}

		return false;
	}

}
