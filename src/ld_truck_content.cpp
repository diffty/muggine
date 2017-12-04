#include "ld_truck_content.hpp"
#include "rsc_manager.hpp"


TruckContent::TruckContent(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY)
	: GridWidgetLayout(vPos, sSize, uNbCellX, uNbCellY) {

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
	vect2df_t vCurPos = getRect()->getPos();
	size2df_t vSlotImgSize = m_pSlotBgImg->getSize();

	for (int i = 0; i < m_uNbCellX; i++) {
		for (int j = 0; j < m_uNbCellY; j++) {
			m_pSlotBgImg->draw(
				pBuffer,
				vCurPos.x + (vSlotImgSize.w - 1) * i,
				vCurPos.y + (vSlotImgSize.h - 1) * j,
				false, true
			);
		}
	}

	GridWidgetLayout::draw(pBuffer);
}

bool TruckContent::isFull() {
	return m_llChildrenWidgets.size >= (m_uNbCellX * m_uNbCellY);
}
