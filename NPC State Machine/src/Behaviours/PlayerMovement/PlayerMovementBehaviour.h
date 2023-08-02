#pragma once

#include "Behaviours/Behaviour.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	struct Node;

	enum class MOVE_DIRECTIONS
	{
		LEFT = 0,
		RIGHT,
		DOWN,
		UP,
		DIR_MAX
	};

	class PlayerMovementBehaviour : public Behaviour
	{
	public:
		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

	private:
		Node *FindLastNodeInDirection(Agent *agent, MOVE_DIRECTIONS direction);

	};

}
