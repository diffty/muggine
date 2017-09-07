#ifndef FSM_HPP
#define FSM_HPP

#include "linked_list.hpp"
#include "fsm_node.hpp"


class FSM {
private:
	LinkedList m_llNodeList;
	FSMNode* m_pActiveNode;
	void (*m_pOnStateChangeCallback)(void) = NULL;

public:
	FSM(void (*pOnStateChangeCallback)(void) = NULL);
	~FSM();

	void addNode(FSMNode* pNewFsmNode);
	FSMNode* getActiveState();

	void update();
};

#endif