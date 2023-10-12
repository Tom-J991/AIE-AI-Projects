#pragma once

#include <memory>

#include "ResourceManager.h"

namespace AIForGames
{
	class NodeMap;
}

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
	bool m_avatar1Dead = false, m_avatar2Dead = false;

	int m_avatar1Health = 4;
	int m_avatar2Health = 4;

	ResourceManager *m_resourceManager;

	AIForGames::NodeMap *m_nodeMap;

};
