#include "grid_widget_layout.hpp"



GridWidgetLayout::GridWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {

	m_uNbCellX = uNbCellX;
	m_uNbCellY = uNbCellY;
}

GridWidgetLayout::~GridWidgetLayout() {
	destroyAllWidgets();
}

void GridWidgetLayout::addWidget(IWidget* pWidget) {
	addChildWidget(pWidget);
	moveWidgetToGrid(m_llChildrenWidgets.size - 1);
}

int GridWidgetLayout::getWidgetIdInLayout(IWidget* pWidget) {
	LLNode* currNode = m_llChildrenWidgets.pHead;
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
	LLNode* currNode = getNodeInList(&m_llChildrenWidgets, uWidgetId);
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
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->update();
		currNode = currNode->pNext;
	}

	updateChildren();
}

void GridWidgetLayout::draw(uint8* buffer) {
	if (m_bIsActive) {
		drawChildren(buffer);

		LLNode* currNode = m_llChildrenWidgets.pHead;
	
		while (currNode != NULL) {
			((IWidget*)currNode->pData)->draw(buffer);
		
			currNode = currNode->pNext;
		}
	}
}

void GridWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}

void GridWidgetLayout::destroyAllWidgets() {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = ((IWidget*)currNode->pData);

		delete pCurrWidget;

		currNode = currNode->pNext;
	}

	clearList(&m_llChildrenWidgets);
}