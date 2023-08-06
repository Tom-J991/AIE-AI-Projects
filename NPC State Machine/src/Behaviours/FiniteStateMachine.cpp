#include "FiniteStateMachine.h"

#include "Pathfinding/Agent.h"
#include "State.h"

#include "Conditions/Condition.h"

namespace AIForGames
{
	FiniteStateMachine::~FiniteStateMachine()
	{
		for (State *s : m_states)
			delete s;
	}

	void FiniteStateMachine::Enter(Agent *agent)
	{
		m_currentState->Enter(agent);
	}

	void FiniteStateMachine::Update(Agent *agent, float deltaTime)
	{
		State *newState = nullptr;

		for (State::Transition t : m_currentState->GetTransitions())
		{
			if (t.condition->IsTrue(agent))
				newState = t.targetState;
		}

		if (newState != nullptr && newState != m_currentState)
		{
			if (m_currentState)
				m_currentState->Exit(agent);
			m_currentState = newState;
			m_currentState->Enter(agent);
		}

		m_currentState->Update(agent, deltaTime);
	}

	void FiniteStateMachine::Exit(Agent *agent)
	{
		m_currentState->Exit(agent);
	}

	void FiniteStateMachine::AddState(State *state)
	{
		m_states.push_back(state);
	}

}
