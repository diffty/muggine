#include "fsm_event.hpp"


FSMEvent::FSMEvent(char* sName, FSMNode* pConnectedNode, bool (*pTestCallback)(void*), void* pTestCallbackArg) {
	m_sName = sName;
	m_pConnectedNode = pConnectedNode;
	m_pTestCallback = pTestCallback;
	m_pTestCallbackArg = pTestCallbackArg;
}

FSMEvent::~FSMEvent() {
	
}

bool FSMEvent::testCondition() {
	return (*m_pTestCallback)(m_pTestCallbackArg);
}

FSMNode* FSMEvent::getConnectedNode() {
	return m_pConnectedNode;
}