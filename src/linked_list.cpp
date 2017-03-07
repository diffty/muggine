#include "linked_list.hpp"


void initList(LinkedList* pList) {
	pList->pHead = NULL;
	pList->pTail = NULL;
	pList->size = 0;
}

void addNodeToList(LinkedList* pList, LLNode* nodeToAdd) {
	if (pList->pHead == NULL) {
		pList->pHead = nodeToAdd;
		pList->pTail = nodeToAdd;
		nodeToAdd->pNext = NULL;
	}
	else {
		pList->pTail->pNext = nodeToAdd;
		pList->pTail = nodeToAdd;
		nodeToAdd->pNext = NULL;
	}

	pList->size++;
}

void removeNodeFromList(LinkedList* pList) {

}

LLNode* searchForNodeInList(LinkedList* pList, LLNode* pNode) {
	if (pList->pHead == NULL) {
		return NULL;
	}

	LLNode* pCurrNode = pList->pHead;

	while(pCurrNode != NULL) {
		if (pCurrNode == pNode) {
			return pCurrNode;
		}
		pCurrNode = pCurrNode->pNext;
	}
    
    return NULL;
}

int countNodesInList(LinkedList* pList) {
	if (pList->pHead == NULL) {
		return 0;
	}

	int nbNodes = 0;
	LLNode* pCurrNode = pList->pHead;

	while(pCurrNode->pNext != NULL) {
		nbNodes++;
		pCurrNode = pCurrNode->pNext;
	}

	return nbNodes;
}
