#include "ts_draggable_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_main_character.hpp"


DraggableThing::DraggableThing(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable, bool bInStore) :
	DraggableSprite(pImg, vPos, pInputManager, bIsDraggable) {

	init(pThingsManager, iAppealPower, iOccupationTime, iCooldownTime, iActionRadius, bUsableOnce, bSingleUser, bInStore);
}

DraggableThing::DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable, bool bInStore) :
	DraggableSprite(pSprSht, uFrameNb, vPos, pInputManager, bIsDraggable) {

	init(pThingsManager, iAppealPower, iOccupationTime, iCooldownTime, iActionRadius, bUsableOnce, bSingleUser, bInStore);
}

DraggableThing::~DraggableThing() {

}


void DraggableThing::init(ThingsManager* pThingsManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bInStore) {
	m_classInfo.setClassTypeName("DraggableThing");

	m_pThingsManager = pThingsManager;

	m_iAppealPower = iAppealPower;
	m_iOccupationTime = iOccupationTime;
	m_iCooldownTime = iCooldownTime;
	m_iActionRadius = iActionRadius;
	m_bUsableOnce = bUsableOnce;
	m_bSingleUser = bSingleUser;
	m_bInStore = bInStore;
	m_bUsableOnlyDuringWork = false;

	m_iMoneyValue = -1;
	m_iPrice = 0;
	m_pCharOwner = NULL;

	initList(&m_llUsers);
}

void DraggableThing::registerNewUser(MainCharacter* pCharUser) {
	addDataToList(&m_llUsers, pCharUser);
}

void DraggableThing::unregisterNewUser(MainCharacter* pCharUser) {
	LLNode* pCurrNode = m_llUsers.pHead;

	while (pCurrNode != NULL) {
		MainCharacter* pCurrCharUser = (MainCharacter*) pCurrNode->pData;

		if (pCharUser == pCurrCharUser) {
			removeNodeFromList(&m_llUsers, pCurrNode);
			delete pCurrNode;
		}

		pCurrNode = pCurrNode->pNext;
	}
}

bool DraggableThing::isUsed() {
	return m_llUsers.size > 0;
}

void DraggableThing::setIsInStore(bool bInStore) {
	m_bInStore = bInStore;
}

bool DraggableThing::isInStore() {
	return m_bInStore;
}

bool DraggableThing::isSingleUser() {
	return m_bSingleUser;
}

bool DraggableThing::isUsableOnlyDuringWork() {
	return m_bUsableOnlyDuringWork;
}

bool DraggableThing::isWorkThing() {
	return m_bIsWorkThing;
}

int DraggableThing::getPrice() {
	return m_iPrice;
}

MainCharacter* DraggableThing::getCharOwner() {
	return m_pCharOwner;
}

void DraggableThing::setMoneyValue(int iMoneyValue) {
	m_iMoneyValue = iMoneyValue;
}

void DraggableThing::setPrice(int iPrice) {
	m_iPrice = iPrice;
}

void DraggableThing::setCharOwner(MainCharacter* pCharOwner) {
	m_pCharOwner = pCharOwner;
}

void DraggableThing::setDestroyAfterUse(bool bDestroyAfterUse) {
	m_bDestroyAfterUse = bDestroyAfterUse;
}

void DraggableThing::setUsableOnlyDuringWork(bool bUsableOnlyDuringWork) {
	m_bUsableOnlyDuringWork = bUsableOnlyDuringWork;
}

void DraggableThing::setIsWorkThing(bool bIsWorkThing) {
	m_bIsWorkThing = bIsWorkThing;
}

void DraggableThing::onDragStart(vect2d_t vStartDragPt) {
	
}

void DraggableThing::onDragEnd() {
	m_pThingsManager->onThingMoved();

	if (m_bInStore) {
		if (TSGameMode::get()->getMoney() - m_iPrice >= 0) {
			m_pThingsManager->addThing(this);
			m_pThingsManager->renewThingInStore(this);
			m_bInStore = false;
			TSGameMode::get()->decreaseMoney(m_iPrice);
		}
		else {
			m_pThingsManager->replaceThingInStore(this);
		}
	}
}

void DraggableThing::onDragging() {
	
}

void DraggableThing::onBeginUsing() {
	
}

void DraggableThing::onEndUsing(MainCharacter* pChar) {
	if (m_iMoneyValue != -1)
		TSGameMode::get()->increaseMoney(m_iMoneyValue);

	printf("%i, %i\n", m_bIsWorkThing, pChar->hasWork());

	if (m_bIsWorkThing && pChar->hasWork()) {
		printf("c ok\n");
		pChar->onEndWork();
	}

	if (m_bDestroyAfterUse) {
		m_bIsActive = false;
		// TODO: faire un appel au thingsmanager pour qu'il delete l'objet au prochain update
	}
}

void DraggableThing::onUsing() {
	
}

int DraggableThing::getOccupationTime() {
	return m_iOccupationTime;
}

int DraggableThing::getActionRadius() {
	return m_iActionRadius;
}

int DraggableThing::getCooldownTime() {
	return m_iCooldownTime;
}