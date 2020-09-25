#include "paginated_widget_layout.hpp"


PaginatedWidgetLayout::PaginatedWidgetLayout(vect2df_t vPos, size2df_t sSize) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {
	
	initList(&m_llWidgetList);
	
	m_pCurrWidget = NULL;
	m_pCurrWidgetId = -1;
}

PaginatedWidgetLayout::~PaginatedWidgetLayout() {
	destroyAllWidgets();
	clearList(&m_llWidgetList);
}

void PaginatedWidgetLayout::addWidget(IWidget* pWidget) {
	addChildWidget(pWidget);

	addDataToList(&m_llWidgetList, pWidget);

	if (m_pCurrWidget == NULL) {
		m_pCurrWidget = pWidget;
		m_pCurrWidgetId = 0;
	}
	else {
		pWidget->setActive(false);
	}
}

void PaginatedWidgetLayout::switchWidget(uint uNum) {
	LLNode* pNode = getNodeInList(&m_llWidgetList, uNum);
	if (pNode != NULL) {
		m_pCurrWidget->setActive(false);
		m_pCurrWidget = (IWidget*) pNode->pData;
		m_pCurrWidget->setActive(true);
		m_pCurrWidgetId = uNum;
	}
}

void PaginatedWidgetLayout::switchToPrevWidget() {
	int iNewWidgetId = (m_pCurrWidgetId - 1);

	if (iNewWidgetId < 0)
		iNewWidgetId = m_llWidgetList.size - 1;

	switchWidget(iNewWidgetId);
}

void PaginatedWidgetLayout::switchToNextWidget() {
	switchWidget((m_pCurrWidgetId + 1) % m_llWidgetList.size);
}

void PaginatedWidgetLayout::update() {
	updateChildren();

	/*if (m_pCurrWidget != NULL) {
		m_pCurrWidget->update();
	}*/
}

void PaginatedWidgetLayout::draw(drawbuffer* pBuffer) {
	if (m_bIsActive) {
		drawChildren(pBuffer);

		/*if (m_pCurrWidget != NULL) {
			m_pCurrWidget->draw(pBuffer);
		}*/
	}
}

bool PaginatedWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	return IWidget::receiveTouchInput(inputPos);

	/*if (m_pCurrWidget != NULL) {
		m_pCurrWidget->receiveTouchInput(inputPos);
	}*/
}

void PaginatedWidgetLayout::updateChildren() {
	IWidget::update();

	/*LLNode* currNode = m_llWidgetList.pHead;

	IWidget* pRootWidget = getRootWidget();

	while (currNode != NULL) {
		IWidget* pCurrWidget = ((IWidget*)currNode->pData);

		pCurrWidget->setRootWidget(pRootWidget);
		pCurrWidget->updateChildren();

		currNode = currNode->pNext;
	}*/
}

void PaginatedWidgetLayout::destroyAllWidgets() {
	LLNode* currNode = m_llWidgetList.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = ((IWidget*) currNode->pData);

		delete pCurrWidget;

		currNode = currNode->pNext;
	}

	clearList(&m_llWidgetList);
}
