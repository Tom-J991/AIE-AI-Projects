#pragma once

#include "Behaviours/Behaviour.h"

#include <vector>

namespace AIForGames
{
	class Agent;
	class Condition;

	class State
	{
	public:
		struct Transition
		{
			Condition *condition;
			State *targetState;

		};

	public:
		State();
		State(Behaviour *behaviour);
		virtual ~State();

		virtual void Enter(Agent *agent);
		virtual void Update(Agent *agent, float deltaTime);
		virtual void Exit(Agent *agent);

		virtual void AddTransition(Condition *condition, State *targetState);

		std::vector<Transition> GetTransitions() const { return m_transitions; }

	private:
		std::vector<Behaviour*> m_behaviours;
		std::vector<Transition> m_transitions;

	};

}
