#pragma once

#include "Behaviours/Behaviour.h"

#include <vector>

namespace AIForGames
{
	class State;

	class FiniteStateMachine : public Behaviour
	{
	public:
		FiniteStateMachine(State *s) : m_currentState(s), m_newState(nullptr) { }
		virtual ~FiniteStateMachine();

		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual void AddState(State *state);

	private:
		std::vector<State*> m_states;
		State *m_currentState;
		State *m_newState;

	};

}
