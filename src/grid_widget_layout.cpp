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
	pWidget->setParentWidget(this);
	moveWidgetToGrid(pWidget, m_llChildrenWidgets.size - 1);
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

void GridWidgetLayout::setGridOffsetX(int x) {
	m_iGridOffsetX = x;
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
	vect2df_t vPosInGrid = getPosInGrid(pWidget, uGridPos);
	pWidget->translate(
		vPosInGrid.x,
		vPosInGrid.y,
		TRANSFORM_ABS
	);
}

vect2df_t GridWidgetLayout::getPosInGrid(IWidget* pWidget, uint uGridPos) {
	vect2d_t vPosInGrid;

	int posXOnGrid = uGridPos % m_uNbCellX;
	int posYOnGrid = uGridPos / m_uNbCellX;

	int iGridCellW = (m_rect.getSize().w / m_uNbCellX);
	int iGridCellH = (m_rect.getSize().h / m_uNbCellY);

	size2df_t sCurrWidgetSize = pWidget->getRect()->getSize();

	return {
		m_rect.getPos().x + iGridCellW * posXOnGrid + iGridCellW / 2 - sCurrWidgetSize.w / 2 + m_iGridOffsetX * posYOnGrid,
		m_rect.getPos().y + iGridCellH * posYOnGrid + iGridCellH / 2 - sCurrWidgetSize.h / 2
	};
}

vect2df_t GridWidgetLayout::getNextPosInGrid(IWidget* pWidget) {
	return getPosInGrid(pWidget, m_llChildrenWidgets.size);
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

/*void GridWidgetLayout::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		((IWidget*)currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}*/

void GridWidgetLayout::destroyAllWidgets() {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = ((IWidget*)currNode->pData);

		delete pCurrWidget;

		currNode = currNode->pNext;
	}

	clearList(&m_llChildrenWidgets);
}