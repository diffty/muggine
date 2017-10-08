#include "fsm.hpp"

FSM::FSM(void (*pOnStateChangeCallback)(void* arg), void* pOnStateChangeCallbackArgs) {
	initList(&m_llNodeList);
	m_pActiveNode = NULL;
	m_pOnStateChangeCallback = pOnStateChangeCallback;
	m_pOnStateChangeCallbackArgs = pOnStateChangeCallbackArgs;
}

FSM::~FSM() {

}

void FSM::addNode(FSMNode* pNewFsmNode) {
	if (m_llNodeList.size == 0) {
		m_pActiveNode = pNewFsmNode;
	}

	LLNode* newNode = new LLNode;
	newNode->pData = pNewFsmNode;

	addNodeToList(&m_llNodeList, newNode);
}

void FSM::update() {
	FSMEvent* pTriggeredEvent = m_pActiveNode->testAllConditions();

	if (pTriggeredEvent != NULL) {
		changeState(pTriggeredEvent->getConnectedNode());
	}
}

void FSM::changeState(FSMNode* pNewStateNode) {
	m_pActiveNode = pNewStateNode;

	printf("changeState: %s\n", m_pActiveNode->getName());

	if (m_pOnStateChangeCallback != NULL) {
		(*m_pOnStateChangeCallback)(m_pOnStateChangeCallbackArgs);
	}
}

void FSM::changeState(int iNewStateId) {
	LLNode* pCurrNode = m_llNodeList.pHead;

	while (pCurrNode != NULL) {
		FSMNode* pStateNode = (FSMNode*) pCurrNode->pData;

		if (pStateNode->getStateId() == iNewStateId) {
			changeState(pStateNode);
			return;
		}

		pCurrNode = pCurrNode->pNext;
	}
}

FSMNode* FSM::getActiveState() {
	return m_pActiveNode;
}
