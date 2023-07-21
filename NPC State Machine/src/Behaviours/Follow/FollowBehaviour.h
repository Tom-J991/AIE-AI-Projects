#pragma once

#include "Behaviours/Behaviour.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	class FollowBehaviour : public Behaviour
	{
	public:
		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

	private:
		glm::vec2 lastTargetPosition;

	};

}
