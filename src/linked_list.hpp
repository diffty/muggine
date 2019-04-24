#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP


#include <iostream>
#include "common_types.hpp"


typedef struct LLNode {
	void* pData;
	LLNode *pNext;
} LLNode;

typedef struct LinkedList {
	LLNode* pHead;
	LLNode* pTail;
	unsigned int size;
} LinkedList;


void initList(LinkedList* pList);
void clearList(LinkedList* pList);
void appendList(LinkedList* pllSrcList, LinkedList* pllDstList);
void addNodeToList(LinkedList* pList, LLNode* nodeToAdd, bool bAddToHead = false);
void addAfterNodeInList(LinkedList* pList, LLNode* pNodeToAdd, LLNode* pPrevNodeInList);
void addDataToList(LinkedList* pList, void* pDataToAdd);
LLNode* removeNodeFromList(LinkedList* pList, LLNode* pNode);
LLNode* searchForNodeInList(LinkedList* pList, void* pDataToSearch);
int countNodesInList(LinkedList* pList);
LLNode* getNodeInList(LinkedList* pList, uint nodeNum);
LLNode* popNodeFromList(LinkedList* pList);

#endif
