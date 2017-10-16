#include "ts_things_store_page.hpp"



ThingsStorePage::ThingsStorePage(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	GridWidgetLayout(vPos, sSize, uNbCellX, uNbCellY) {

}


ThingsStorePage::~ThingsStorePage() {

}

/*void ThingsStorePage::renewThing(DraggableThing* pThingToRenew) {
	LLNode* currNode = m_llContentList.pHead;
	uint uWidgetId = 0;

	while (currNode != NULL) {
		DraggableThing* pCurrThing = (DraggableThing*)currNode->pData;

		if (pCurrThing == pThingToRenew) {
			DraggableThing* pNewClonedThing = new DraggableThing(*pCurrThing);

			pNewClonedThing->setIsInStore(true);

			currNode->pData = (void*)pNewClonedThing;
			Scene* parentScene = pCurrThing->getParentScene();
			pCurrThing->getParentScene()->addComponent(pCurrThing);

			break;
		}

		uWidgetId++;
		currNode = currNode->pNext;
	}

	moveWidgetToGrid(uWidgetId);
}*/

void ThingsStorePage::renewThing(DraggableThing* pThingToRenew) {
	uint uWidgetId = 0;

	ThingsStoreItem* pThingWidget = getThingItemInLayout(pThingToRenew);

	DraggableThing* pNewClonedThing = new DraggableThing(*pThingToRenew);
	pNewClonedThing->setIsInStore(true);
	getParentScene()->addComponent(pThingToRenew);

	pThingWidget->replaceThing(pNewClonedThing);
}

uint ThingsStorePage::countWidgets() {
	return m_llContentList.size;
}

int ThingsStorePage::getThingWidgetIdInLayout(DraggableThing* pThing) {
	LLNode* currNode = m_llContentList.pHead;
	int i = 0;

	while (currNode != NULL) {
		ThingsStoreItem* currWidget = (ThingsStoreItem*)currNode->pData;

		if (currWidget->getThing() == pThing)
			return i;

		currNode = currNode->pNext;
		i++;
	}
	return -1;
}

ThingsStoreItem* ThingsStorePage::getThingItemInLayout(DraggableThing* pThing) {
	LLNode* currNode = m_llContentList.pHead;
	int i = 0;

	while (currNode != NULL) {
		ThingsStoreItem* currWidget = (ThingsStoreItem*)currNode->pData;

		if (currWidget->getThing() == pThing)
			return currWidget;

		currNode = currNode->pNext;
		i++;
	}
}

void ThingsStorePage::updateChildren() {
	LLNode* currNode = m_llContentList.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)currNode->pData;
		pCurrWidget->setParentScene(getParentScene());
		pCurrWidget->updateChildren();

		currNode = currNode->pNext;
	}
}