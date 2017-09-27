#ifndef FSM_NODE_HPP
#define FSM_NODE_HPP

#include "linked_list.hpp"
#include "fsm_event.hpp"


class FSMNode {
private:
	char* m_sName;
	int m_iStateId;
	LinkedList m_llConnectedEventList;

public:
	FSMNode(int iStateId, char* sName);
	~FSMNode();

	void addEvent(char* sName, FSMNode* pConnectedNode, bool (*pTestCallback)(void*), void* pTestCallbackArg);
	FSMEvent* testAllConditions();
	char* getName();
	int getStateId();
};

#endif