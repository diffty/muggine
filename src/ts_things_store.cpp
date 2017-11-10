#include "ts_things_store.hpp"
#include "ts_things_store_page.hpp"

ThingsStore::ThingsStore(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY) :
	PaginatedWidgetLayout(vPos, sSize) {
	
	m_uNbCellX = uNbCellX;
	m_uNbCellY = uNbCellY;

	createNewPage();

	vect2df_t vBtnPos;

	vBtnPos.x = 290;
	vBtnPos.y = 210;
	m_pPageUpStoreBtn = new ImageButtonWidget(RscManager::get()->getSprShtRsc(6), vBtnPos, 1, 0, 0, onPressPageUpBtnCallback, this);

	vBtnPos.x = 305;
	vBtnPos.y = 210;
	m_pPageDownStoreBtn = new ImageButtonWidget(RscManager::get()->getSprShtRsc(6), vBtnPos, 3, 2, 0, onPressPageDownBtnCallback, this);
}

ThingsStore::~ThingsStore() {
	//delete m_pPageUpStoreBtn;
	//delete m_pPageDownStoreBtn;
}

// sale
void ThingsStore::addPageBtnComponentsToMainScene() {
	getParentScene()->addComponent(m_pPageUpStoreBtn);
	getParentScene()->addComponent(m_pPageDownStoreBtn);
}

void ThingsStore::addThingToStore(DraggableThing* pThing) {
	ThingsStorePage* lastPage = ((ThingsStorePage*)m_llWidgetList.pTail->pData);
	ThingsStoreItem* pNewStoreItem = new ThingsStoreItem(pThing);

	if (lastPage != NULL) {
		if (lastPage->countWidgets() >= 7) {
			createNewPage();
			lastPage = ((ThingsStorePage*)m_llWidgetList.pTail->pData);
		}

		lastPage->addWidget(pNewStoreItem);
		pNewStoreItem->updateChildrenPosition();
	}
}

void ThingsStore::createNewPage() {
	ThingsStorePage* pNewPage = new ThingsStorePage(m_rect.getPos(), m_rect.getSize(), m_uNbCellX, m_uNbCellY);
	addWidget(pNewPage);
}

bool ThingsStore::findThingInPages(DraggableThing* pThing, ThingsStorePage** pLayoutWithThing, int* piThingWidgetId) {

	LLNode* currNode = m_llWidgetList.pHead;

	while (currNode != NULL) {
		ThingsStorePage* pCurrPageLayout = (ThingsStorePage*) currNode->pData;

		if (pCurrPageLayout != NULL) {
			int iThingWidgetId = pCurrPageLayout->getThingWidgetIdInLayout(pThing);

			if (iThingWidgetId != -1) {
				*piThingWidgetId = iThingWidgetId;
				*pLayoutWithThing = pCurrPageLayout;
				return true;
			}
		}

		currNode = currNode->pNext;
	}

	return false;
}

DraggableThing* ThingsStore::renewThing(DraggableThing* pThingToRenew) {
	ThingsStorePage* pLayoutWithThing = NULL;
	int piThingWidgetId;

	if (findThingInPages(pThingToRenew, &pLayoutWithThing, &piThingWidgetId)) {
		return pLayoutWithThing->renewThing(pThingToRenew);
	}

	return NULL;
}

void ThingsStore::replaceThingInStore(DraggableThing* pThingToMove) {
	ThingsStorePage* pLayoutWithThing = NULL;
	int piThingWidgetId;

	if (findThingInPages(pThingToMove, &pLayoutWithThing, &piThingWidgetId)) {
		ThingsStoreItem* pThingWidget = pLayoutWithThing->getThingItemInLayout(pThingToMove);
		pThingWidget->replaceThing(pThingToMove);
	}
}

void ThingsStore::updateChildren() {
	LLNode* currNode = m_llWidgetList.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = (IWidget*) currNode->pData;
		pCurrWidget->setParentScene(getParentScene());
		pCurrWidget->updateChildren();

		currNode = currNode->pNext;
	}
}

void ThingsStore::onPressPageUpBtnCallback(void* pArg) {
	ThingsStore* pThingsStore = ((ThingsStore*) pArg);
	pThingsStore->switchToPrevWidget();
}

void ThingsStore::onPressPageDownBtnCallback(void* pArg) {
	ThingsStore* pThingsStore = ((ThingsStore*)pArg);
	pThingsStore->switchToNextWidget();
}
