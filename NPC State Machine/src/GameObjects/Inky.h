#pragma once

#include "Ghost.h"

class Inky : public Ghost
{
public:
	Inky(NodeMap *nodeMap, Node *startNode);
	virtual ~Inky() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw(Texture &texture) override;

private:

};
