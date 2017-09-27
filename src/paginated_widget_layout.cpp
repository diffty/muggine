#include "paginated_widget_layout.hpp"



PaginatedWidgetLayout::PaginatedWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {

	m_uNbCellX = uNbCellX;
	m_uNbCellY = uNbCellY;

	initList(&m_llContentList);
}

PaginatedWidgetLayout::~PaginatedWidgetLayout() {

}

void PaginatedWidgetLayout::addWidget(IWidget* pWidget) {
	LLNode* newNode = new LLNode;
	newNode->pData = pWidget;

	pWidget->setParentScene(getParentScene());

	addNodeToList(&m_llContentList, newNode);
	moveWidgetToGrid(m_llContentList.size - 1);
}

void PaginatedWidgetLayout::moveWidgetToGrid(uint uWidgetId) {
	LLNode* currNode = getNodeInList(&m_llContentList, uWidgetId);
	IWidget* currWidget = (IWidget*) currNode->pData;

	uint posXOnGrid = uWidgetId % m_uNbCellX;
	uint posYOnGrid = uWidgetId / m_uNbCellX;

	int iGridCellW = (m_rect.getSize().w / m_uNbCellX);
	int iGridCellH = (m_rect.getSize().h / m_uNbCellY);

	size2df_t sCurrWidgetSize = currWidget->getRect()->getSize();

	currWidget->translate(m_rect.getPos().x + iGridCellW * posXOnGrid + iGridCellW / 2 - sCurrWidgetSize.w / 2,
						  m_rect.getPos().y + iGridCellH * posYOnGrid + iGridCellH / 2 - sCurrWidgetSize.h / 2,
						  TRANSFORM_ABS);
}

void PaginatedWidgetLayout::update() {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->update();
		currNode = currNode->pNext;
	}
}

void PaginatedWidgetLayout::draw(uint8* fb) {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->draw(fb);
		currNode = currNode->pNext;
	}
}

void PaginatedWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}

void PaginatedWidgetLayout::updateChildren() {
	LLNode* currNode = m_llContentList.pHead;

	Scene* pParentScene = getParentScene();

	while (currNode != NULL) {
		IWidget* pCurrentWidget = ((IWidget*)currNode->pData);

		pCurrentWidget->setParentScene(pParentScene);
		pCurrentWidget->updateChildren();

		currNode = currNode->pNext;
	}
}