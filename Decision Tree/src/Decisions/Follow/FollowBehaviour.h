#pragma once

#include "Decisions/Decision.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	class FollowBehaviour : public Decision
	{
	public:
		FollowBehaviour();
		virtual ~FollowBehaviour();

		virtual void MakeDecision(Agent *agent, float deltaTime) override;

	private:
		glm::vec2 lastTargetPosition;

	};

}
