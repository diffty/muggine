#include "paginated_widget_layout.hpp"


PaginatedWidgetLayout::PaginatedWidgetLayout(vect2df_t vPos, size2df_t sSize) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {

	initList(&m_llWidgetList);
	pCurrWidget = NULL;
	pCurrWidgetId = -1;
}

PaginatedWidgetLayout::~PaginatedWidgetLayout() {
	destroyAllWidgets();
	//clearList(&m_llWidgetList);
}

void PaginatedWidgetLayout::addWidget(IWidget* pWidget) {
	addDataToList(&m_llWidgetList, pWidget);

	pWidget->setParentScene(getParentScene());

	if (pCurrWidget == NULL) {
		pCurrWidget = pWidget;
		pCurrWidgetId = 0;
	}
}

void PaginatedWidgetLayout::switchWidget(uint uNum) {
	LLNode* pNode = getNodeInList(&m_llWidgetList, uNum);
	if (pNode != NULL) {
		pCurrWidget = (IWidget*) pNode->pData;
		pCurrWidgetId = uNum;
	}
}

void PaginatedWidgetLayout::switchToPrevWidget() {
	int iNewWidgetId = (pCurrWidgetId - 1);

	if (iNewWidgetId < 0)
		iNewWidgetId = m_llWidgetList.size - 1;

	switchWidget(iNewWidgetId);
}

void PaginatedWidgetLayout::switchToNextWidget() {
	switchWidget((pCurrWidgetId + 1) % m_llWidgetList.size);
}

void PaginatedWidgetLayout::update() {
	if (pCurrWidget != NULL) {
		pCurrWidget->update();
	}
}

void PaginatedWidgetLayout::draw(uint8* fb) {
	if (pCurrWidget != NULL) {
		pCurrWidget->draw(fb);
	}
}

void PaginatedWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	if (pCurrWidget != NULL) {
		pCurrWidget->receiveTouchInput(inputPos);
	}
}

void PaginatedWidgetLayout::updateChildren() {
	LLNode* currNode = m_llWidgetList.pHead;

	Scene* pParentScene = getParentScene();

	while (currNode != NULL) {
		IWidget* pCurrWidget = ((IWidget*)currNode->pData);

		pCurrWidget->setParentScene(pParentScene);
		pCurrWidget->updateChildren();

		currNode = currNode->pNext;
	}
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