#include "HealthCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool HealthCondition::IsTrue(Agent *agent, float deltaTime)
	{
		// Returns true if the avatar's health meets the given condition based on the given value.
		return (*m_avatarHealth > m_value) == m_greaterThan;
	}

}
