#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP


#include <iostream>


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
void addNodeToList(LinkedList* pList, LLNode* nodeToAdd);
void removeNodeFromList(LinkedList* pList);
LLNode* searchForNodeInList(LinkedList* pList, LLNode* pNode);
int countNodesInList(LinkedList* pList);


#endif