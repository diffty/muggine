#include "ld_game_mode.hpp"
#include "ld_game_manager.hpp"
#include "rsc_manager.hpp"


LDGameMode* LDGameMode::s_pInstance = NULL;


LDGameMode::LDGameMode(Scene* pMainScene, LinkedList llObjectsOwned, int iStartMoney, int iStartHealth, int iGiveNFreeObjects)
	: m_objectsData("data/objects_data.csv") {

	LDGameMode::s_pInstance = this;

	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;

	initScene();

	size2df_t iSlotSize = RscManager::get()->getImgRsc(10)->getSize();

	m_pTruckContent = new TruckContent({ 12, 105 }, { 2 * (iSlotSize.w - 1), 1 * (iSlotSize.h - 1) }, 2, 1);
	m_pTruckContent->setDrawOrder(1000);
	m_pTruckContent->setParentWidget(m_pMainScene);

	m_llObjectsOwned = llObjectsOwned;

	setMoney(iStartMoney);
	setHealth(iStartHealth);
	setTimer(10);

	initList(&m_llWidgetTrash);

	giveFreeObjects(iGiveNFreeObjects);
	buyObjects();

	Object* pPickedObj;
	if (pPickedObj = pickObject()) {
		m_pCurrCard = spawnNewCard(pPickedObj);
		m_pCurrCard->setDraggable(true);

		if (pPickedObj = pickObject()) {
			m_pNextCard = m_pCurrCard;
			m_pNextCard->setDraggable(false);

			m_pCurrCard = spawnNewCard(pPickedObj);
			m_pCurrCard->setDraggable(true);
		}
	}
	
	m_bLevelEnded = false;
}

LDGameMode::~LDGameMode() {
	m_pMainScene->garbageCollect();
	m_pMainScene->destroy();
}

void LDGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
	System* sys = System::get();

	m_pBgSpr = new Sprite(rscManager->getImgRsc(6), {0, 0});
	m_pBgSpr->setDrawOrder(10);
	m_pBgSpr->setParentWidget(m_pMainScene);

	m_pTimerLabel = new Text("0", rscManager->getFontRsc(0), { 52, 75 });
	m_pHealthLabel = new Text("100", rscManager->getFontRsc(0), { 155, 75 });
	m_pMoneyLabel = new Text("0", rscManager->getFontRsc(0), { 242, 75 });

	m_pTimerLabel->setDrawOrder(10);
	m_pHealthLabel->setDrawOrder(10);
	m_pMoneyLabel->setDrawOrder(10);

	m_pTimerLabel->setParentWidget(m_pMainScene);
	m_pHealthLabel->setParentWidget(m_pMainScene);
	m_pMoneyLabel->setParentWidget(m_pMainScene);

	m_pTimerSpr = new Sprite(rscManager->getSprShtRsc(8), 0, { 35, 76 });;
	m_pHealthSpr = new Sprite(rscManager->getSprShtRsc(8), 1, { 138, 76 });
	m_pMoneySpr = new Sprite(rscManager->getSprShtRsc(8), 2, { 225, 76 });

	m_pTimerSpr->setDrawOrder(10);
	m_pHealthSpr->setDrawOrder(10);
	m_pMoneySpr->setDrawOrder(10);

	m_pTimerSpr->setParentWidget(m_pMainScene);
	m_pHealthSpr->setParentWidget(m_pMainScene);
	m_pMoneySpr->setParentWidget(m_pMainScene);
}

void LDGameMode::giveFreeObjects(int nbObjects) {
	for (int i = 0; i < nbObjects; i++) {
		Object* pNewObj = generateNewObject();
		addDataToList(&m_llObjectsOwned, pNewObj);
	}
}

void LDGameMode::buyObjects() {
	while (m_iMoney > 0) {
		Object* pNewObj = generateNewObject();

		if (m_iMoney - pNewObj->getPrice() < 0) {
			delete pNewObj;
			return;
		}
		else {
			addDataToList(&m_llObjectsOwned, pNewObj);
			decreaseMoney(pNewObj->getPrice());
			increaseHealth(pNewObj->getLoveFactor());
		}
	}
}

Object* LDGameMode::pickObject() {
	if (m_llObjectsOwned.pHead) {
		Object* pPickedObject = (Object*)m_llObjectsOwned.pHead->pData;
		removeNodeFromList(&m_llObjectsOwned, m_llObjectsOwned.pHead);
		return pPickedObject;
	}
	return NULL;
}

Object* LDGameMode::generateNewObject() {
	srand((uint)((System::get()->getTime() - (long)System::get()->getTime()) * 1000000000));

	int newObjId = rand() % m_objectsData.countDataRows();

	char* szName = m_objectsData.getData("Name", newObjId);
	int iPriceMin = intFromStr(m_objectsData.getData("PriceMin", newObjId));
	int iPriceMax = intFromStr(m_objectsData.getData("PriceMax", newObjId));
	int iSprShtFrameId = intFromStr(m_objectsData.getData("SprShtFrameId", newObjId));

	Object* pNewObj = new Object(szName, RscManager::get()->getSprShtRsc(9), iSprShtFrameId, { 0, 0 }, false);
	pNewObj->setPrice(iPriceMin + rand() % (iPriceMax - iPriceMin));
	pNewObj->setLoveFactor(rand() % 5);

	return pNewObj;
}

ObjectCard* LDGameMode::spawnNewCard(Object* pNewObj) {
	ObjectCard* pNewCard = new ObjectCard(RscManager::get()->getImgRsc(7), { 125, 106 }, false);
	pNewCard->setObject(pNewObj);
	pNewCard->setParentWidget(m_pMainScene);
	pNewCard->setDraggable(false);

	return pNewCard;
}

void LDGameMode::update() {
	if (System::get()->getInputSys()->IsKeyPressed(KEYB_A)) {
	
	}
	
	if (!m_bLevelEnded) {
		float fNewTimer = m_fTimer - System::get()->getDeltaTime();

		if (fNewTimer > 0) {
			setTimer(fNewTimer);
		}
		else {
			m_bLevelEnded = true;
			onLevelEndBeforeSell();
		}
	}

	if (m_bSellingLeftObj) {
		if (m_pCurrCard) {
			m_fTimeBeforeSellNextObj -= System::get()->getDeltaTime();

			if (m_fTimeBeforeSellNextObj <= 0) {
				Object* pObj = m_pCurrCard->getObject();

				pObj->parentToRoot();

				pObj->goToSell();

				destroyObjectCard(m_pCurrCard);

				m_fTimeBeforeSellNextObj = 0.5;
			}
		}
	}

	destroyWidgetsInTrash();
}

Scene* LDGameMode::getScene() {
	return m_pMainScene;
}

TruckContent* LDGameMode::getTruckContent() {
	return m_pTruckContent;
}

void LDGameMode::destroyObjectCard(ObjectCard* pObjCard) {
	if (m_pCurrCard == pObjCard) { // mmmh...
		if (m_pNextCard) {
			m_pNextCard->setDraggable(true);
		}

		m_pCurrCard = m_pNextCard;

		if (m_pCurrCard) {
			m_pCurrCard->setDrawOrder(5100);
		}

		Object* pPickedObj;
		if (pPickedObj = pickObject()) {
			m_pNextCard = spawnNewCard(pPickedObj);
			m_pNextCard->setDrawOrder(5050);
		}
		else {
			m_pNextCard = NULL;
		}
	}

	pObjCard->getParentWidget()->removeChildWidget(pObjCard);
	addDataToList(&m_llWidgetTrash, pObjCard);
}

void LDGameMode::onObjKeep(Object* pObj) {
	if (!m_pCurrCard) {
		onLevelEndAfterSell();
	}
}

void LDGameMode::onObjSold(Object* pObj) {
	increaseMoney(pObj->getPrice());
	decreaseHealth(pObj->getLoveFactor() * 3);

	if (!m_pCurrCard) {
		onLevelEndAfterSell();
	}

	pObj->setParentWidget(NULL);
	addDataToList(&m_llWidgetTrash, pObj);
}

void LDGameMode::onLevelEndBeforeSell() {
	if (m_llObjectsOwned.size > 0) {
		m_bSellingLeftObj = true;
		m_fTimeBeforeSellNextObj = 0.3;
	}
}

void LDGameMode::onLevelEndAfterSell() {
	LDGameManager::get()->onLevelSuccess();
}

void LDGameMode::onNoMoreCard() {
	onLevelEndAfterSell();
}

int LDGameMode::getMoney() {
	return m_iMoney;
}

int LDGameMode::getHealth() {
	return m_iHealth;
}

float LDGameMode::getTimer() {
	return m_fTimer;
}

LinkedList LDGameMode::getObjectsOwnedList() {
	return m_llObjectsOwned;
}

void LDGameMode::increaseMoney(int iMoneyBonus) {
	setMoney(m_iMoney + iMoneyBonus);
}

void LDGameMode::decreaseMoney(int iMoneyMalus) {
	setMoney(m_iMoney - iMoneyMalus);
}

void LDGameMode::increaseHealth(int iHealthBonus) {
	setHealth(m_iHealth + iHealthBonus);
}

void LDGameMode::decreaseHealth(int iHealthMalus) {
	setHealth(m_iHealth - iHealthMalus);
}

void LDGameMode::setMoney(int iMoney) {
	m_iMoney = iMoney;
	updateMoneyLabel();
}

void LDGameMode::setHealth(int iHealth) {
	m_iHealth = iHealth;
	updateHealthLabel();
}

void LDGameMode::setTimer(float fTimer) {
	m_fTimer = fTimer;
	updateTimerLabel();
}

void LDGameMode::updateHealthLabel() {
	m_pHealthLabel->setText(m_iHealth);
}

void LDGameMode::updateMoneyLabel() {
	m_pMoneyLabel->setText(m_iMoney);
}

void LDGameMode::updateTimerLabel() {
	m_pTimerLabel->setText((int) m_fTimer);
}

void LDGameMode::destroyWidgetsInTrash() {
	LLNode* pCurrNode = m_llWidgetTrash.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrCard = (IWidget*)pCurrNode->pData;
		delete pCurrCard;
		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llWidgetTrash);
}

LDGameMode* LDGameMode::get() {
	return LDGameMode::s_pInstance;
}

void LDGameMode::initGameMode(LDGameMode** pGameMode, Scene* pScene, int iGiveNFreeObjects) {
	LinkedList llObjectsOwned;
	int iStartMoney = 0;
	int iStartHealth = 100;

	if (*pGameMode) {
		llObjectsOwned = (*pGameMode)->getTruckContent()->getObjectsList();
		iStartMoney = (*pGameMode)->getMoney();
		iStartHealth = (*pGameMode)->getHealth();
		(*pGameMode)->getTruckContent()->unlinkAllWidgets();
		delete *pGameMode;
		
	}
	else {
		initList(&llObjectsOwned);
	}

	IWidget* w = (IWidget*)llObjectsOwned.pHead;

	*pGameMode = new LDGameMode(pScene, llObjectsOwned, iStartMoney, iStartHealth, iGiveNFreeObjects);
}
