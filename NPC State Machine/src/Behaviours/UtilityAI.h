#pragma once

#include "Behaviours/Behaviour.h"

#include <vector>

namespace AIForGames
{
	class UtilityAI : public Behaviour
	{
	public:
		virtual ~UtilityAI();

		virtual void Update(Agent *agent, float deltaTime) override;

		virtual void AddBehaviour(Behaviour *behaviour);

	private:
		std::vector<Behaviour*> m_behaviours;
		Behaviour *m_currentBehaviour;

	};

}
