#pragma once

namespace AIForGames
{
	class Agent;

	class Condition
	{
	public:
		virtual bool IsTrue(Agent *agent) = 0;

	};

}
