#include "fsm.hpp"

FSM::FSM(void(*pOnStateChangeCallback)(void)) {
	initList(&m_llNodeList);
	m_pActiveNode = NULL;
	m_pOnStateChangeCallback = pOnStateChangeCallback;
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
		m_pActiveNode = pTriggeredEvent->getConnectedNode();

		if (m_pOnStateChangeCallback != NULL) {
			(*m_pOnStateChangeCallback)();
		}
	}
}

FSMNode* FSM::getActiveState() {
	return m_pActiveNode;
}