#pragma once

#include <memory>

#include "ResourceManager.h"

#include "GameObjects/Player.h"
#include "GameObjects/Blinky.h"
#include "GameObjects/Pinky.h"
#include "GameObjects/Inky.h"
#include "GameObjects/Clyde.h"

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Cleanup();

	void Update(float deltaTime);
	void Draw();

private:
	void IntroSequence(float deltaTime);

private:
	int m_highScore = 0;
	int m_playerScore = 0;
	int m_playerLives = 4;
	int m_gameLevel = 0;

	bool m_gameStart = false;
	bool m_introOne = false;
	bool m_introTwo = false;

	ResourceManager *m_resourceManager;

	std::unique_ptr<Player> m_player;
	std::unique_ptr<Blinky> m_blinky;
	std::unique_ptr<Pinky> m_pinky;
	std::unique_ptr<Inky> m_inky;
	std::unique_ptr<Clyde> m_clyde;

	NodeMap *m_nodeMap;

};
