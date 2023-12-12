#pragma once

#include "Decisions/Decision.h"

namespace AIForGames
{
	class WanderBehaviour : public Decision
	{
	public:
		WanderBehaviour();
		virtual ~WanderBehaviour();

		virtual void MakeDecision(Agent *agent, float deltaTime) override;

	};

}
