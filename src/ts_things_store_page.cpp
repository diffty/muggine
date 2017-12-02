#include "ts_things_store_page.hpp"



ThingsStorePage::ThingsStorePage(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	GridWidgetLayout(vPos, sSize, uNbCellX, uNbCellY) {

}


ThingsStorePage::~ThingsStorePage() {

}

DraggableThing* ThingsStorePage::renewThing(DraggableThing* pThingToRenew) {
	uint uWidgetId = 0;

	ThingsStoreItem* pThingWidget = getThingItemInLayout(pThingToRenew);

	DraggableThing* pNewClonedThing = NULL;

	if (pThingToRenew->getClassType()->getClassTypeName() == "WinningThing")
		pNewClonedThing = new WinningThing(*((WinningThing*) pThingToRenew));
	else {
		pNewClonedThing = new DraggableThing(*pThingToRenew);
	}

	pNewClonedThing->init(pThingToRenew->getTitle(), pThingToRenew->getDesc());
	pNewClonedThing->setIsInStore(true);
	pNewClonedThing->setParentWidget(pThingWidget);

	pThingWidget->replaceThing(pNewClonedThing);

	return pNewClonedThing;
}

uint ThingsStorePage::countWidgets() {
	return m_llChildrenWidgets.size;
}

int ThingsStorePage::getThingWidgetIdInLayout(DraggableThing* pThing) {
	LLNode* currNode = m_llChildrenWidgets.pHead;
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
	LLNode* currNode = m_llChildrenWidgets.pHead;
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
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)currNode->pData;
		pCurrWidget->setRootWidget(getRootWidget());
		pCurrWidget->updateChildren();

		currNode = currNode->pNext;
	}
}