#pragma once

namespace AIForGames
{
	class Agent;

	class Decision
	{
	public:
		Decision() = default;
		virtual ~Decision() = default;

		virtual void MakeDecision(Agent *agent, float deltaTime) = 0;

	};
}
