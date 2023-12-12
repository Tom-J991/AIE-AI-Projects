#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <vector>

#include "PathAgent.h"

namespace AIForGames
{
	struct Node;
	class NodeMap;
	class Decision;

	inline bool CompareColor(Color a, Color b)
	{
		bool compareRed = (a.r == b.r);
		bool compareGreen = (a.g == b.g);
		bool compareBlue = (a.b == b.b);
		bool compareAlpha = (a.a == b.a);

		return (compareRed && compareGreen && compareBlue && compareAlpha);
	}

	class Agent
	{
	public:
		Agent() { }
		Agent(NodeMap *nodeMap, Decision *decision);
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

		float GetSpeed() const { return m_pathAgent.GetSpeed(); }
		void SetSpeed(float speed) { m_pathAgent.SetSpeed(speed); }

		glm::vec2 GetPosition() const { return m_pathAgent.GetPosition(); }
		void SetPosition(glm::vec2 position) { m_pathAgent.SetPosition(position); }

		void SetColor(Color color) { m_color = color; }
		Color GetColor() const { return m_color; }

		void SetSize(float size) { m_size = size; }

		Node *GetCurrentNode() const { return m_pathAgent.GetCurrentNode(); }

	private:
		Agent *m_target = nullptr;
		PathAgent m_pathAgent;
		Decision *m_current = nullptr;
		NodeMap *m_nodeMap = nullptr;
		Color m_color;
		float m_size = 16;

	};

}
