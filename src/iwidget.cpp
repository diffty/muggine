#include "iwidget.hpp"


IWidget::~IWidget() {
	//printf("delete widget \n");
	if (m_pParentWidget) {
		m_pParentWidget->removeChildWidget(this);
	}

	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*) pCurrNode->pData;
		delete pCurrWidget;
		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llChildrenWidgets);

	garbageCollect();
};

void IWidget::drawChildren(uint8* buffer) {
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		if (pCurrWidget->isActive()) pCurrWidget->draw(buffer);
		pCurrNode = pCurrNode->pNext;
	}
}

void IWidget::updateChildren() {
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		if (pCurrWidget->isActive()) pCurrWidget->update();
		pCurrNode = pCurrNode->pNext;
	}

	garbageCollect();
}

bool IWidget::receiveTouchInputChildren(vect2d_t touchPt) {
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	bool bInputHit = false;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		if (pCurrWidget->isActive()) {
			bInputHit = pCurrWidget->receiveTouchInput(touchPt);

			if (bInputHit && m_bStopOnFirstInput) {
				break;
			}
		}
		pCurrNode = pCurrNode->pNext;
	}
	return bInputHit;
}

void IWidget::translate(float x, float y, ETransformMode transformMode) {
    vect2df_t currPos = m_rect.getPos();

    vect2df_t vDeltaPos;
    if (transformMode == TRANSFORM_ABS) {
        vDeltaPos.x = x - currPos.x;
        vDeltaPos.y = y - currPos.y;
    }
    else {
        vDeltaPos.x = x;
        vDeltaPos.y = y;
    }
    
	if (transformMode == TRANSFORM_REL) {
		currPos.x += x;
		currPos.y += y;
	}
	else {
		currPos.x = x;
		currPos.y = y;
	}
	m_rect.setPos(currPos.x, currPos.y);
    
    // Move children too (assuming their pos is relative to parent)
    LLNode* pCurrNode = m_llChildrenWidgets.pHead;

    while (pCurrNode != NULL) {
        IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
        
        pCurrWidget->translate(vDeltaPos.x, vDeltaPos.y, TRANSFORM_REL);
        pCurrNode = pCurrNode->pNext;
    }
}

void IWidget::setActive(bool bIsActive) {
	m_bIsActive = bIsActive;
}

bool IWidget::isActive() {
	return m_bIsActive;
}

void IWidget::setRootWidget(IWidget* pRootWidget) {
	m_pRootWidget = pRootWidget;
	setChildrenRootWidget(pRootWidget);
}

void IWidget::setChildrenRootWidget(IWidget* pRootWidget) {
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		pCurrWidget->setRootWidget(pRootWidget);
		pCurrNode = pCurrNode->pNext;
	}
}

IWidget* IWidget::getRootWidget() {
	return m_pRootWidget;
}

void IWidget::addChildWidget(IWidget* pWidget) {
	if (searchForNodeInList(&m_llChildrenWidgets, pWidget) == NULL) {
		addWidgetToDrawOrder(pWidget);
		pWidget->setRootWidget(getRootWidget());

		/*printf("* New Draw Order :\n");

		LLNode* pCurrNode = m_llChildrenWidgets.pHead;

		while (pCurrNode != NULL) {
			IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
			printf("%p: %i\n", pCurrWidget, pCurrWidget->getDrawOrder());
			pCurrNode = pCurrNode->pNext;
		}
		printf("--**--\n");*/
	}
	else {
		printf("<!> Widget %p already in children !\n", pWidget);
	}
}

void IWidget::removeChildWidget(IWidget* pWidget) {
	LLNode* pNode = searchForNodeInList(&m_llChildrenWidgets, pWidget);
	if (pNode) {
		removeNodeFromList(&m_llChildrenWidgets, pNode);
		addDataToList(&m_llWidgetNodesToDelete, pNode);
	}
}

void IWidget::unlinkAllWidgets() {
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode) {
		((IWidget*)pCurrNode->pData)->setParentWidget(NULL);
		pCurrNode = pCurrNode->pNext;
	}
	
	clearList(&m_llChildrenWidgets);
}

void IWidget::garbageCollect() {
	LLNode* pCurrNode = m_llWidgetNodesToDelete.pHead;
	LLNode* pNextCurr;

	while (pCurrNode != NULL) {
		pNextCurr = pCurrNode->pNext;
		delete (LLNode*) pCurrNode->pData;
		pCurrNode = pNextCurr;
	}

	clearList(&m_llWidgetNodesToDelete);
}

void IWidget::setParentWidget(IWidget* pWidget) {
	if (m_pParentWidget != NULL) {
		m_pParentWidget->removeChildWidget(this);
	}

	if (pWidget != NULL) {
		pWidget->addChildWidget(this);
		printf("<i> %p parented to %p. New root : %p\n", this, pWidget, m_pRootWidget);
		m_pParentWidget = pWidget;
	}
	else {
		m_pParentWidget = NULL;
		m_pRootWidget = NULL;
	}
}

IWidget* IWidget::getParentWidget() {
	return m_pParentWidget;
}

void IWidget::setDrawOrder(int iNewDrawOrder) {
	m_iDrawOrder = iNewDrawOrder;
	updateWidgetInParentDrawOrder();
}

int IWidget::getDrawOrder() {
	return m_iDrawOrder;
}

void IWidget::addWidgetToDrawOrder(IWidget* pWidgetToAdd) {
	printf("<i> Add widget %p to draw order of %p.\n", pWidgetToAdd, this);

	LLNode* pCurrNode = m_llChildrenWidgets.pHead;
	LLNode* pPrevNode = NULL;

	if (m_llChildrenWidgets.pHead == NULL) {
		addDataToList(&m_llChildrenWidgets, pWidgetToAdd);
		//printf("!%p: %i! (first)\n", pWidgetToAdd, pWidgetToAdd->getDrawOrder());
		return;
	}

	IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;

	if (pCurrWidget->getDrawOrder() > pWidgetToAdd->getDrawOrder()) {
		LLNode* pNewNode = new LLNode;
		pNewNode->pNext = m_llChildrenWidgets.pHead;
		pNewNode->pData = pWidgetToAdd;
		m_llChildrenWidgets.pHead = pNewNode;
		//printf("!%p: %i! (first)\n", pWidgetToAdd, pWidgetToAdd->getDrawOrder());
		return;
	}

	while (pCurrNode != NULL) {
		pCurrWidget = (IWidget*)pCurrNode->pData;

		//printf("%p: %i, ", pCurrWidget, pCurrWidget->getDrawOrder());

		if (pCurrWidget->getDrawOrder() > pWidgetToAdd->getDrawOrder()) {
			LLNode* pNewNode = new LLNode;
			pNewNode->pNext = NULL;
			pNewNode->pData = pWidgetToAdd;
			if (pPrevNode) {
				addAfterNodeInList(&m_llChildrenWidgets, pNewNode, pPrevNode);
			}
			else {
				pNewNode->pNext = m_llChildrenWidgets.pHead;
				m_llChildrenWidgets.pHead = pNewNode;
			}
			//printf("<- !%p: %i!\n", pWidgetToAdd, pWidgetToAdd->getDrawOrder());
			return;
		}

		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}

	//printf("!%p: %i! (last)\n", pWidgetToAdd, pWidgetToAdd->getDrawOrder());
	addDataToList(&m_llChildrenWidgets, pWidgetToAdd);
}

void IWidget::updateWidgetInParentDrawOrder() {
	if (m_pParentWidget) {
		m_pParentWidget->updateWidgetInDrawOrder(this);
	}
	else {
		printf("<?> No parent for draw order update of %p.\n", this);
	}
}

void IWidget::updateWidgetInDrawOrder(IWidget* pWidget) {
	LLNode* pWidgetNode = searchForNodeInList(&m_llChildrenWidgets, pWidget);

	if (pWidgetNode) {
		updateWidgetNodeInDrawOrder(pWidgetNode);
	}
	else {
		printf("<!> Widget %p not found in children of %p for draw order update.\n", pWidget, this);
	}
}

void IWidget::updateWidgetNodeInDrawOrder(LLNode* pWidgetNodeToUpdate) {
	LLNode* pPrevNode = NULL;
	LLNode* pCurrNode = m_llChildrenWidgets.pHead;
	IWidget* pWidgetToUpdate = (IWidget*)pWidgetNodeToUpdate->pData;
	
	bool bNodeToUpdateDisconnected = false;
	bool bNodeToUpdateReconnected = false;

	LLNode* pOldNextNode = pWidgetNodeToUpdate->pNext;

	while (pCurrNode != NULL) {
		LLNode* pNextNode = pCurrNode->pNext;

		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		IWidget* pPrevWidget = NULL;
		IWidget* pNextWidget = NULL;

		if (pNextNode) {
			pNextWidget = (IWidget*) pNextNode->pData;
		}

		if (pPrevNode) {
			pPrevWidget = (IWidget*) pPrevNode->pData;
		}

		if (pCurrNode == pWidgetNodeToUpdate) {
			bool bAllIsOk = true;

			if (pPrevWidget) {
				if (!(pPrevWidget->getDrawOrder() < pWidgetToUpdate->getDrawOrder())) {
					bAllIsOk = false;
				}
			}
			
			if (pNextWidget) {
				if (!(pWidgetToUpdate->getDrawOrder() < pNextWidget->getDrawOrder())) {
					bAllIsOk = false;
				}
			}

			if (bAllIsOk && !bNodeToUpdateDisconnected) {
				//printDrawOrder();
				return;
			}
			else {
				bNodeToUpdateDisconnected = true;
				//printf("disconnected\n");

				if (pPrevNode) {
					pPrevNode->pNext = pOldNextNode;
				}
				else {
					m_llChildrenWidgets.pHead = pWidgetNodeToUpdate->pNext;
				}

				if (pWidgetNodeToUpdate->pNext == NULL) {
					m_llChildrenWidgets.pTail = pPrevNode;
				}
			}
		}
		else if (!bNodeToUpdateReconnected) {
			if (pCurrWidget->getDrawOrder() >= pWidgetToUpdate->getDrawOrder()) {
				if (pPrevNode) {
					pPrevNode->pNext = pWidgetNodeToUpdate;
					pWidgetNodeToUpdate->pNext = pCurrNode;
					pCurrNode = pWidgetNodeToUpdate->pNext;
				}
				else {
					pWidgetNodeToUpdate->pNext = m_llChildrenWidgets.pHead;
					m_llChildrenWidgets.pHead = pWidgetNodeToUpdate;
				}

				bNodeToUpdateReconnected = true;
				/*if (pPrevNode)
					printf("reconnected to %p\n", pPrevNode->pData);
				else
					printf("reconnected\n");*/
			}
		}

		if (bNodeToUpdateReconnected && bNodeToUpdateDisconnected) {
			//printDrawOrder();
			return;
		}
		
		if (pCurrNode != pWidgetNodeToUpdate) {
			pPrevNode = pCurrNode;
		}
		pCurrNode = pCurrNode->pNext;
	}

	if (bNodeToUpdateDisconnected && !bNodeToUpdateReconnected) {
		if (pPrevNode) {
			pPrevNode->pNext = pWidgetNodeToUpdate;
			pWidgetNodeToUpdate->pNext = NULL;
			//printf("reconnected to %p\n", pPrevNode->pData);
		}
	}

	//printDrawOrder();
}

void IWidget::printDrawOrder() {
	printf("* Current Draw Order :\n");

	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)pCurrNode->pData;
		printf("%p: %i\n", pCurrWidget, pCurrWidget->getDrawOrder());
		pCurrNode = pCurrNode->pNext;
	}
	printf("--**--\n");
}
