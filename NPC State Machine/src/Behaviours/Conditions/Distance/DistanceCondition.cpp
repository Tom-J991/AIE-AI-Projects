#include "DistanceCondition.h"

#include "Pathfinding/Agent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	bool DistanceCondition::IsTrue(Agent *agent, float deltaTime)
	{
		return (glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition()) < m_distance) == m_lessThan;
	}

}
