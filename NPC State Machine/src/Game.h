#pragma once

#include <memory>

namespace AIForGames
{
	class NodeMap;
}

class Game
{
public:
	Game(int difficulty = 1);
	~Game();

	void Init();
	void Cleanup();

	void Update(float deltaTime);
	void Draw();

private:
	bool m_avatar1Dead = false, m_avatar2Dead = false;

	int m_avatar1MaxHP = 0;
	int m_avatar1Health = 0;
	bool m_avatar1IsAttacking = false;

	int m_avatar2MaxHP = 0;
	int m_avatar2Health = 0;
	bool m_avatar2IsAttacking = false;

	bool m_gameover = false;

	AIForGames::NodeMap *m_nodeMap;

};
