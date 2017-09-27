#include "ts_things_manager.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_things_store.hpp"


ThingsManager::ThingsManager(ThingsStore* pStore) {
	initList(&m_llThingsList);
	m_pStore = pStore;
}


ThingsManager::~ThingsManager() {

}


void ThingsManager::addThing(DraggableThing* pNewThing) {
	addDataToList(&m_llThingsList, pNewThing);
}

void ThingsManager::getClosestAvailableThingsToPoint(LinkedList* pllAvailableThings, vect2d_t vPos) {
	LLNode* pCurrNode = m_llThingsList.pHead;
	
	float fMinDistance = 9999999;

	while (pCurrNode != NULL) {
		DraggableThing* pCurrThing = (DraggableThing*) pCurrNode->pData;
		vect2df_t vCurrThingPos = pCurrThing->getRect()->getPos();
		float fCurrDistance = sqrt(powf(vPos.x - vCurrThingPos.x, 2) + powf(vPos.y - vCurrThingPos.y, 2));

		LLNode* pClonedNode = new LLNode(*pCurrNode);
		pClonedNode->pNext = NULL;

		LLNode* pCurrSortedNode = pllAvailableThings->pHead;
		LLNode* pPrevSortedNode = NULL;

		float fPrevCurrSortedThingDist = -1;

		printf("** %f\n", fCurrDistance);

		while (pCurrSortedNode != NULL) {
			DraggableThing* pCurrSortedThing = (DraggableThing*) pCurrSortedNode->pData;
			vect2df_t vCurrSortedThingPos = pCurrSortedThing->getRect()->getPos();
			float fCurrSortedThingDist = sqrt(powf(vPos.x - vCurrSortedThingPos.x, 2) + powf(vPos.y - vCurrSortedThingPos.y, 2));

			printf("%f, %f, %p\n", fCurrDistance, fCurrSortedThingDist, pClonedNode->pData);

			if (fCurrDistance < fCurrSortedThingDist)  {
				printf("FOUND\n");
				break;
			}

			pPrevSortedNode = pCurrSortedNode;
			fPrevCurrSortedThingDist = fCurrSortedThingDist;

			pCurrSortedNode = pCurrSortedNode->pNext;
		}

		if (pPrevSortedNode == NULL) {
			pClonedNode->pNext = pllAvailableThings->pHead;
			pllAvailableThings->pHead = pClonedNode;
		}
		else {
			pClonedNode->pNext = pPrevSortedNode->pNext;
			pPrevSortedNode->pNext = pClonedNode;
		}

		printf("-----\n");

		pCurrNode = pCurrNode->pNext;
	}
}

void ThingsManager::renewThingInStore(DraggableThing* pThingToRenew) {
	m_pStore->renewThing(pThingToRenew);
}
