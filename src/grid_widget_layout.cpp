#include "grid_widget_layout.hpp"



GridWidgetLayout::GridWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {

	m_uNbCellX = uNbCellX;
	m_uNbCellY = uNbCellY;

	initList(&m_llContentList);
}

GridWidgetLayout::~GridWidgetLayout() {

}

void GridWidgetLayout::addWidget(IWidget* pWidget) {
	LLNode* newNode = new LLNode;
	newNode->pData = pWidget;

	pWidget->setParentScene(getParentScene());

	addNodeToList(&m_llContentList, newNode);
	moveWidgetToGrid(m_llContentList.size - 1);
}

int GridWidgetLayout::getWidgetIdInLayout(IWidget* pWidget) {
	LLNode* currNode = m_llContentList.pHead;
	int i = 0;

	while (currNode != NULL) {
		IWidget* currWidget = (IWidget*)currNode->pData;

		if (currWidget == pWidget)
			return i;

		currNode = currNode->pNext;
		i++;
	}
	return -1;
}

void GridWidgetLayout::moveWidgetToGrid(uint uWidgetId) {
	LLNode* currNode = getNodeInList(&m_llContentList, uWidgetId);
	IWidget* currWidget = (IWidget*)currNode->pData;

	moveWidgetToGrid(currWidget, uWidgetId);
}

void GridWidgetLayout::moveWidgetToGrid(IWidget* pWidget) {
	int iWidgetId = getWidgetIdInLayout(pWidget);

	if (iWidgetId != -1)
		moveWidgetToGrid(pWidget, iWidgetId);
}

void GridWidgetLayout::moveWidgetToGrid(IWidget* pWidget, uint uGridPos) {
	uint posXOnGrid = uGridPos % m_uNbCellX;
	uint posYOnGrid = uGridPos / m_uNbCellX;

	int iGridCellW = (m_rect.getSize().w / m_uNbCellX);
	int iGridCellH = (m_rect.getSize().h / m_uNbCellY);

	size2df_t sCurrWidgetSize = pWidget->getRect()->getSize();

	pWidget->translate(m_rect.getPos().x + iGridCellW * posXOnGrid + iGridCellW / 2 - sCurrWidgetSize.w / 2,
						  m_rect.getPos().y + iGridCellH * posYOnGrid + iGridCellH / 2 - sCurrWidgetSize.h / 2,
						  TRANSFORM_ABS);
}

void GridWidgetLayout::update() {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->update();
		currNode = currNode->pNext;
	}
}

void GridWidgetLayout::draw(uint8* fb) {
	LLNode* currNode = m_llContentList.pHead;
	
	while (currNode != NULL) {
		((IWidget*)currNode->pData)->draw(fb);
		
		currNode = currNode->pNext;
	}
}

void GridWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}

void GridWidgetLayout::updateChildren() {
	LLNode* currNode = m_llContentList.pHead;

	Scene* pParentScene = getParentScene();

	while (currNode != NULL) {
		IWidget* pCurrentWidget = ((IWidget*)currNode->pData);

		pCurrentWidget->setParentScene(pParentScene);
		pCurrentWidget->updateChildren();

		currNode = currNode->pNext;
	}
}