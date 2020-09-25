#include "grid_widget_layout.hpp"



GridWidgetLayout::GridWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h) {

	m_uNbCellX = uNbCellX;
	m_uNbCellY = uNbCellY;

	m_iGridOffsetX = 0.;
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
	int iPosXOnGrid = uGridPos % m_uNbCellX;
	int iPosYOnGrid = uGridPos / m_uNbCellX;

	long lGridCellW = (m_rect.getSize().w / m_uNbCellX);
	long lGridCellH = (m_rect.getSize().h / m_uNbCellY);

	size2df_t sCurrWidgetSize = pWidget->getRect()->getSize();
	vect2df_t vPos;
	vPos.x = m_rect.getPos().x + lGridCellW * iPosXOnGrid + lGridCellW / 2 - sCurrWidgetSize.w / 2 + m_iGridOffsetX * iPosYOnGrid;
	vPos.y = m_rect.getPos().y + lGridCellH * iPosYOnGrid + lGridCellH / 2 - sCurrWidgetSize.h / 2;
	
	return vPos;
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

void GridWidgetLayout::draw(drawbuffer* pBuffer) {
	if (m_bIsActive) {
		drawChildren(pBuffer);

		LLNode* currNode = m_llChildrenWidgets.pHead;
	
		while (currNode != NULL) {
			((IWidget*)currNode->pData)->draw(pBuffer);
		
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
