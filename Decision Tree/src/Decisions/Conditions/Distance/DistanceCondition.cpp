#include "DistanceCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool DistanceCondition::IsTrue(Agent *agent, float deltaTime)
	{
		// Returns true if the avatar's distance from the target meets the given condition.
		return (glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition()) < m_distance) == m_lessThan;
	}

}