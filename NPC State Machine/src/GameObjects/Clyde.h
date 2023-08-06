#pragma once

#include "Ghost.h"

class Clyde : public Ghost
{
public:
	Clyde(NodeMap *nodeMap, Node *startNode);
	virtual ~Clyde() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw(Texture &texture) override;

private:

};
