#ifndef FSM_EVENT_HPP
#define FSM_EVENT_HPP

#include <iostream>


class FSMNode;

class FSMEvent {
private:
	char* m_sName;
	FSMNode* m_pConnectedNode;
	bool (*m_pTestCallback)(void*);
	void* m_pTestCallbackArg;

public:
	FSMEvent(char* sName, FSMNode* pConnectedNode, bool (*pTestCallback)(void*) = NULL, void* pTestCallbackArg = NULL);
	~FSMEvent();

	bool testCondition();
	FSMNode* getConnectedNode();
};

#include "fsm_node.hpp"

#endif