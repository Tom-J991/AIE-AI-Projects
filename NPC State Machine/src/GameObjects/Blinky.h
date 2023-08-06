#pragma once

#include "Ghost.h"

class Blinky : public Ghost
{
public:
	Blinky(NodeMap *nodeMap, Node *startNode);
	virtual ~Blinky() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw(Texture &texture) override;

private:

};
