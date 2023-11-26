#include "FlagCheckCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool FlagCheckCondition::IsTrue(Agent *agent, float deltaTime)
	{
		// Returns true if the given flag meets the given condition.
		return *m_flagCheck == m_condition;
	}

}
