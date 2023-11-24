#include "HealthCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool HealthCondition::IsTrue(Agent *agent, float deltaTime)
	{
		return (*m_avatarHealth > m_value) == m_greaterThan;
	}

}
