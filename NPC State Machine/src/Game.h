#pragma once

#include <memory>

namespace AIForGames
{
	class NodeMap; // Declaration here just so we can declare a variable of this type below.
}

class Game
{
public:
	Game(int difficulty = 1); // Default difficulty to 1 just in case.
	~Game();

	void Init();
	void Cleanup();

	void Update(float deltaTime);
	void Draw();

private:
	// Game variables.
	bool m_avatar1Dead = false, m_avatar2Dead = false; // Flags for when either avatar is dead.

	int m_avatar1MaxHP = 0; // Max health. (Used for drawing the health bar)
	int m_avatar1Health = 0; // The avatar's health.
	bool m_avatar1IsAttacking = false; // Flag to check whether the avatar is currently attacking.

	int m_avatar2MaxHP = 0; // Same use cases as above just for the second avatar.
	int m_avatar2Health = 0;
	bool m_avatar2IsAttacking = false;

	bool m_gameover = false; // Game over flag.

	AIForGames::NodeMap *m_nodeMap;

};
