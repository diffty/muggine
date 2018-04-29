#include "fsm_node.hpp"


FSMNode::FSMNode(int iStateId, char* sName) {
	m_iStateId = iStateId;
	m_sName = sName;
	initList(&m_llConnectedEventList);
}

FSMNode::~FSMNode() {

}

FSMEvent* FSMNode::testAllConditions() {
	LLNode* currNode = m_llConnectedEventList.pHead;

	while (currNode != NULL) {
		FSMEvent* currEvent = (FSMEvent*) currNode->pData;

		if (currEvent->testCondition()) {
			return currEvent;
		}

		currNode = currNode->pNext;
	}

	return NULL;
}

void FSMNode::addEvent(char* sName, FSMNode* pConnectedNode, bool (*pTestCallback)(void*), void* pTestCallbackArg) {
	LLNode* newNode = new LLNode;
	FSMEvent* newEvent = new FSMEvent(sName, pConnectedNode, pTestCallback, pTestCallbackArg);
	newNode->pData = newEvent;
	addNodeToList(&m_llConnectedEventList, newNode);
}

char* FSMNode::getName() {
	return m_sName;
}

int FSMNode::getStateId() {
	return m_iStateId;
}
