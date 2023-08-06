#pragma once

#include "Ghost.h"

class Pinky : public Ghost
{
public:
	Pinky(NodeMap *nodeMap, Node *startNode);
	virtual ~Pinky() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw(Texture &texture) override;

private:

};
