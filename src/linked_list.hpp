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
void destroyList(LinkedList* pList);
void appendList(LinkedList* pllSrcList, LinkedList* pllDstList);
void addNodeToList(LinkedList* pList, LLNode* nodeToAdd, bool bAddToHead = false);
void addDataToList(LinkedList* pList, void* pDataToAdd);
LLNode* removeNodeFromList(LinkedList* pList, LLNode* pNode);
LLNode* searchForNodeInList(LinkedList* pList, LLNode* pNode);
int countNodesInList(LinkedList* pList);
LLNode* getNodeInList(LinkedList* pList, uint nodeNum);

#endif
