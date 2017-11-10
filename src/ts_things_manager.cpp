#include "ts_things_manager.hpp"
#include "ts_things_store.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_workguy_thing.hpp"
#include "ts_critical_thing.hpp"
#include "ts_winning_thing.hpp"
#include "ts_main_character.hpp"


ThingsManager::ThingsManager(ThingsStore* pStore) {
	initList(&m_llThingsList);
	initList(&m_llThingsToDelete);
	m_pStore = pStore;
}


ThingsManager::~ThingsManager() {
	destroyAllThings();
}


void ThingsManager::addThing(DraggableThing* pNewThing) {
	addDataToList(&m_llThingsList, pNewThing);
}

void ThingsManager::deleteThing(DraggableThing* pThingToDelete) {
	addDataToList(&m_llThingsToDelete, pThingToDelete);
}

void ThingsManager::processThingsToDeleteList() {
	LLNode* pCurrNode = m_llThingsToDelete.pHead;

	while (pCurrNode != NULL) {
		LLNode* pNodeToDelete = searchForNodeInList(&m_llThingsList, pCurrNode->pData);
		if (pNodeToDelete) {
			removeNodeFromList(&m_llThingsList, pNodeToDelete);
			delete pNodeToDelete;
		}

		DraggableThing* pThingToDelete = (DraggableThing*) pCurrNode->pData;
		if (pThingToDelete) {
			m_pStore->getParentScene()->removeComponent(pThingToDelete);
			delete pThingToDelete;
		}

		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llThingsToDelete);
}

void ThingsManager::getClosestAvailableThingsToPoint(LinkedList* pllAvailableThings, vect2d_t vPos, MainCharacter* pChar) {
	LLNode* pCurrNode = m_llThingsList.pHead;
	
	while (pCurrNode != NULL) {
		DraggableThing* pCurrThing = (DraggableThing*)pCurrNode->pData;
		vect2df_t vCurrThingPos = pCurrThing->getRect()->getPos();
		float fCurrDistance = sqrt(powf(vPos.x - vCurrThingPos.x, 2) + powf(vPos.y - vCurrThingPos.y, 2));

		if ((pCurrThing->isSingleUser()
			&& !pCurrThing->isUsed())
			&& ((float) pCurrThing->getActionRadius() > fCurrDistance || pCurrThing->getActionRadius() == -1)
			&& (pCurrThing->getCharOwner() == NULL || pCurrThing->getCharOwner() == pChar)
			&& (!pCurrThing->isUsableOnlyDuringWork() || (pChar != NULL && (pChar->hasWork() && pCurrThing->isUsableOnlyDuringWork())))
			&& !((pCurrThing->getClassType()->getClassTypeName() == "WorkguyThing" && pChar->hasWork()))
			&& pCurrThing->isActive()) {

			LLNode* pClonedNode = new LLNode(*pCurrNode);
			pClonedNode->pNext = NULL;

			LLNode* pCurrSortedNode = pllAvailableThings->pHead;
			LLNode* pPrevSortedNode = NULL;

			float fPrevCurrSortedThingDist = -1;

			while (pCurrSortedNode != NULL) {
				DraggableThing* pCurrSortedThing = (DraggableThing*)pCurrSortedNode->pData;
				vect2df_t vCurrSortedThingPos = pCurrSortedThing->getRect()->getPos();
				float fCurrSortedThingDist = sqrt(powf(vPos.x - vCurrSortedThingPos.x, 2) + powf(vPos.y - vCurrSortedThingPos.y, 2));

				if (fCurrDistance < fCurrSortedThingDist) {
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
		}

		pCurrNode = pCurrNode->pNext;
	}
}

DraggableThing* ThingsManager::renewThingInStore(DraggableThing* pThingToRenew) {
	return m_pStore->renewThing(pThingToRenew);
}

void ThingsManager::replaceThingInStore(DraggableThing* pThingToReplace) {
	m_pStore->replaceThingInStore(pThingToReplace);
}

void ThingsManager::onCriticalThingUsed() {
	TSGameMode::get()->decreaseHealth(0.01);
}

void ThingsManager::onWorkguyThingUsed() {

}

void ThingsManager::onThingMoved() {
	TSGameMode::get()->onThingMoved();
}

void ThingsManager::destroyAllThings() {
	LLNode* pCurrNode = m_llThingsList.pHead;

	while (pCurrNode != NULL) {
		const char* currThingClass = ((IWidget*)pCurrNode->pData)->getClassType()->getClassTypeName();

		if (currThingClass == "CriticalThing") {
			delete ((CriticalThing*)pCurrNode->pData);
		}

		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llThingsToDelete);
	clearList(&m_llThingsList);
}
