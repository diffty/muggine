#include "double_linked_list.hpp"


DLLNode* createNode(void* pData) {
	DLLNode* pNewNode = new DLLNode;
	pNewNode->pData = pData;
	return pNewNode;
}