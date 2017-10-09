#include "ts_things_store.hpp"



ThingsStore::ThingsStore(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	PaginatedWidgetLayout(vPos, sSize, uNbCellX, uNbCellY) {
	
}

ThingsStore::~ThingsStore() {

}

void ThingsStore::renewThing(DraggableThing* pThingToRenew) {
	LLNode* currNode = m_llContentList.pHead;
	uint uWidgetId = 0;

	while (currNode != NULL) {
		DraggableThing* pCurrThing = (DraggableThing*) currNode->pData;

		if (pCurrThing == pThingToRenew) {
			DraggableThing* pNewClonedThing = new DraggableThing(*pCurrThing);

			pNewClonedThing->setIsInStore(true);
			
			currNode->pData = (void*) pNewClonedThing;
			Scene* parentScene = pCurrThing->getParentScene();
			pCurrThing->getParentScene()->addComponent(pCurrThing);

			break;
		}

		uWidgetId++;
		currNode = currNode->pNext;
	}

	moveWidgetToGrid(uWidgetId);
}
