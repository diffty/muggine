#include "ld_truck_content.hpp"
#include "rsc_manager.hpp"


TruckContent::TruckContent(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY)
	: GridWidgetLayout(vPos, sSize, uNbCellX, uNbCellY) {

	setGridOffsetX(-12);
	m_pSlotBgImg = RscManager::get()->getImgRsc(10);
}


TruckContent::~TruckContent() {
	
}

void TruckContent::addObject(Object* pObj) {
	addWidget(pObj);
}

LinkedList TruckContent::getObjectsList() {
	LinkedList llNewList;
	initList(&llNewList);

	LLNode* pCurrNode = m_llChildrenWidgets.pHead;

	while (pCurrNode) {
		addDataToList(&llNewList, (Object*) pCurrNode->pData);
		pCurrNode = pCurrNode->pNext;
	}

	return llNewList;
}

void TruckContent::draw(uint8* pBuffer) {
	GridWidgetLayout::draw(pBuffer);
}

bool TruckContent::isFull() {
	return m_llChildrenWidgets.size >= (m_uNbCellX * m_uNbCellY);
}
