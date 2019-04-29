#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#define NULL nullptr


typedef struct DLLNode {
	void* pData;
	DLLNode *pPrev = NULL;
	DLLNode *pNext = NULL;
} DLLNode;


DLLNode* createNode(void* pData);


class DoubleLinkedList {
public:
	DoubleLinkedList() {};
	~DoubleLinkedList() {};

	DLLNode* addData(void* pData) {
		DLLNode* pNewNode = createNode(pData);
		addNode(pNewNode);
		return pNewNode;
	}

	void addDataBefore(void* pData, DLLNode* pOtherNode = NULL) {
		DLLNode* pNewNode = createNode(pData);
		insertNodeBefore(pNewNode, pOtherNode);
	}

	void addDataAfter(void* pData, DLLNode* pOtherNode = NULL) {
		DLLNode* pNewNode = createNode(pData);
		insertNodeAfter(pNewNode, pOtherNode);
	}

	void addNode(DLLNode* pNode, bool bAddToHead = false) {
		if (m_pHead == NULL) {
			m_pHead = pNode;
			m_pTail = pNode;
			pNode->pNext = NULL;
		}
		else {
			if (bAddToHead) {
				pNode->pNext = m_pHead;
				m_pHead->pPrev = pNode;
				m_pHead = pNode;
			}
			else {
				if (m_pTail != NULL)
					m_pTail->pNext = pNode;

				pNode->pPrev = m_pTail;
				m_pTail = pNode;
				pNode->pNext = NULL;
			}
		}

		m_iSize++;
	}

	void insertNodeAfter(DLLNode* pNode, DLLNode* pPrevNode) {
		pNode->pNext = pPrevNode->pNext;
		pNode->pPrev = pPrevNode;

		if (pNode->pNext != NULL)
			pNode->pNext->pPrev = pNode;

		pPrevNode->pNext = pNode;

		m_iSize++;
	}

	void insertNodeBefore(DLLNode* pNode, DLLNode* pNextNode) {
		pNode->pNext = pNextNode;
		pNode->pPrev = pNextNode->pPrev;

		if (pNode->pPrev != NULL)
			pNode->pPrev->pNext = pNode;

		pNextNode->pPrev = pNode;

		m_iSize++;
	}

	DLLNode* searchData(DoubleLinkedList* pList, void* pDataToSearch, DLLNode* pFromNode=NULL, bool bReverseSearch=false) {

		DLLNode* pCurrNode = (pFromNode==NULL ? (bReverseSearch ? m_pTail : m_pHead) : pFromNode) ;

		while (pCurrNode != NULL) {
			if (pCurrNode->pData == pDataToSearch) {
				return pCurrNode;
			}
			pCurrNode = (bReverseSearch ? pCurrNode->pPrev : pCurrNode->pNext);
		}

		return NULL;
	}

	DLLNode* getHead() {
		return m_pHead;
	}

	DLLNode* getTail() {
		return m_pTail;
	}

private:
	DLLNode* m_pHead = NULL;
	DLLNode* m_pTail = NULL;
	unsigned int m_iSize = 0;
};


#endif // !DOUBLE_LINKED_LIST_HPP
