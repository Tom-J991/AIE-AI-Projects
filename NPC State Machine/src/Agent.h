#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <vector>

#include "PathAgent.h"

#include <glm/glm.hpp>

namespace AIForGames
{
	class Node;
	class NodeMap;
	class Behaviour;

	class Agent
	{
	public:
		Agent() { }
		Agent(NodeMap *nodeMap, Behaviour *behaviour);
		~Agent();

		void Update(float deltaTime);
		void Draw();

		void Reset();

		void GoToPoint(glm::vec2 point);
		void GoToNode(Node *node);
		void SetNode(Node *node) { m_pathAgent.SetNode(node); }

		void SetTarget(Agent *target) { m_target = target; }
		Agent *GetTarget() const { return m_target; }

		std::vector<Node*> &GetPath() { return m_pathAgent.GetPath(); }
		bool PathComplete() { return m_pathAgent.GetPath().empty(); }

		NodeMap *GetNodeMap() const { return m_nodeMap; }

		void SetSpeed(float speed) { m_pathAgent.SetSpeed(speed); }
		glm::vec2 GetPosition() const { return m_pathAgent.GetPosition(); }

		void SetColor(Color color) { m_color = color; }

		void SetSize(float size) { m_size = size; }

	private:
		Agent *m_target;
		PathAgent m_pathAgent;
		Behaviour *m_current;
		NodeMap *m_nodeMap;
		Color m_color;
		float m_size = 16;

	};

}
