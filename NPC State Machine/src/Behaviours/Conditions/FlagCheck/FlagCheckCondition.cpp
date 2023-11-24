#include "FlagCheckCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool FlagCheckCondition::IsTrue(Agent *agent, float deltaTime)
	{
		return *m_flagCheck == m_condition;
	}

}
