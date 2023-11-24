#pragma once

#include "Behaviours/Behaviour.h"

namespace AIForGames
{
	class FleeBehaviour : public Behaviour
	{
	public:
		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

	private:
		float m_storedSpeed = 0.0f;

	};

}
