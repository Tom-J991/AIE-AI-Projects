#include "Ghost.h"

#include <raylib.h>

#include "Behaviours/Wander/WanderBehaviour.h"

Ghost::Ghost(NodeMap *nodeMap, Node *startNode)
{
    m_agent = new Agent(nodeMap, new WanderBehaviour());
    m_agent->SetNode(startNode);
    m_agent->SetColor(RED);
    m_agent->SetSize(8);
    m_agent->SetSpeed(80 * 0.75f);
}
Ghost::~Ghost()
{ 
    //delete m_agent; // TODO: Fix read access error
}
