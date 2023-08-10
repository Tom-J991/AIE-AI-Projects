#include "Ghost.h"

#include <raylib.h>

#include "Behaviours/State.h"

Ghost::Ghost(NodeMap *nodeMap, Node *startNode)
{
    m_stateMachine = new FiniteStateMachine(new State());

    m_agent = new Agent(nodeMap, m_stateMachine);
    m_agent->SetNode(startNode);
    m_agent->SetColor(RED);
    m_agent->SetSize(8);
    m_agent->SetSpeed(80 * 0.75f);
}
Ghost::~Ghost()
{ 
    //delete m_agent; // TODO: Fix read access errors
    //delete m_stateMachine;
    //m_agent = nullptr;
    //m_stateMachine = nullptr;
}
