#pragma once

#include "Behaviours/Behaviour.h"

namespace AIForGames
{
	class SelectorBehaviour : public Behaviour
	{
	public:
		SelectorBehaviour(Behaviour *b1, Behaviour *b2) : m_b1(b1), m_b2(b2) { }

		virtual void Enter(Agent *agent) override;
		virtual void Update(Agent *agent, float deltaTime) override;
		virtual void Exit(Agent *agent) override;

		virtual float Evaluate(Agent *agent) override;

		void SetBehaviour(Behaviour *b, Agent *agent);

	private:
		Behaviour *m_b1;
		Behaviour *m_b2;
		Behaviour *m_selected;

	};

}
