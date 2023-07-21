#pragma once

#include "Behaviours/Behaviour.h"

namespace AIForGames
{
	class GotoPointBehaviour : public Behaviour
	{
	public:
		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

	};

}
