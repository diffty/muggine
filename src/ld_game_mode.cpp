#include "ld_game_mode.hpp"
#include "ld_game_manager.hpp"
#include "rsc_manager.hpp"


#define START_HEALTH            50
#define UPGRADE_BASE_COST       500
#define UPGRADE_COST_PER_LVL    250


LDGameMode* LDGameMode::s_pInstance = NULL;


LDGameMode::LDGameMode(Scene* pMainScene, LinkedList llObjectsOwned, int iStartMoney, int iStartHealth, int iGiveNFreeObjects) {

	LDGameMode::s_pInstance = this;

	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;
    
    printf("test\n");
    char* platformPath = platformConformPath("data/objects_data.csv");
    printf("%s\n", platformPath);
    m_objectsData = new CSVReader(platformPath);
    delete platformPath;

	initScene();

	setMoney(iStartMoney);
	setHealth(iStartHealth);
	setTimer(11);

	// Level up the truck if >UPGRADE_COST$
	int iTruckLevel = LDGameManager::get()->getTruckLevel();
        
    int iCurrUpgradeCost = UPGRADE_BASE_COST + UPGRADE_COST_PER_LVL * (iTruckLevel - 1);
        
	if (m_iMoney > iCurrUpgradeCost && iTruckLevel < 6) {
		LDGameManager::get()->setTruckLevel(iTruckLevel + 1);
		m_iMoney -= iCurrUpgradeCost;
	}

	//size2df_t iSlotSize = RscManager::get()->getImgRsc(10)->getSize();
	size2d_t sTruckSlotSize = calculateTruckSize();
	
	m_pTruck = new Truck({ 12, 105 }, (int) sTruckSlotSize.w, (int) sTruckSlotSize.h);
	m_pTruck->setDrawOrder(1100);
	m_pTruck->setParentWidget(m_pMainScene);

	m_llObjectsOwned = llObjectsOwned;

	initList(&m_llWidgetTrash);

	giveFreeObjects(iGiveNFreeObjects);
	buyObjects();

	Object* pPickedObj;
	if ((pPickedObj = pickObject())) {
		m_pCurrCard = spawnNewCard(pPickedObj);
		m_pCurrCard->setDraggable(true);

		if ((pPickedObj = pickObject())) {
			m_pNextCard = m_pCurrCard;
			m_pNextCard->setDraggable(false);

			m_pCurrCard = spawnNewCard(pPickedObj);
			m_pCurrCard->setDraggable(true);
		}
	}
	
	m_bLevelEnded = false;

	m_pParticleSystem = new ParticleSystem;

	m_pParticleSystem->getRect()->setPos(150, 100);
	m_pParticleSystem->setParentWidget(pMainScene);
	m_pParticleSystem->setDrawOrder(6000);
}

LDGameMode::~LDGameMode() {
	m_pMainScene->garbageCollect();
	m_pMainScene->destroy();
    delete m_objectsData;
}

void LDGameMode::initScene() {
	RscManager* rscManager = RscManager::get();

	m_pBgSpr = new Sprite(rscManager->getImgRsc(6), {0, 0});
	m_pBgSpr->setDrawOrder(10);
	m_pBgSpr->setParentWidget(m_pMainScene);

	m_pTimerSpr = new Sprite(rscManager->getSprShtRsc(8), 0, { 270, 185 });
	m_pHealthSpr = new Sprite(rscManager->getSprShtRsc(8), 1, { 270, 200 });
	m_pMoneySpr = new Sprite(rscManager->getSprShtRsc(8), 2, { 270, 215 });

	m_pTimerSpr->setDrawOrder(10);
	m_pHealthSpr->setDrawOrder(10);
	m_pMoneySpr->setDrawOrder(10);

	m_pTimerSpr->setParentWidget(m_pMainScene);
	m_pHealthSpr->setParentWidget(m_pMainScene);
	m_pMoneySpr->setParentWidget(m_pMainScene);

	m_pTimerLabel = new Text("0", rscManager->getFontRsc(16), {
		m_pTimerSpr->getRect()->getPos().x + 17,
		m_pTimerSpr->getRect()->getPos().y + 2
	});
	m_pHealthLabel = new Text("100", rscManager->getFontRsc(16), {
		m_pHealthSpr->getRect()->getPos().x + 17,
		m_pHealthSpr->getRect()->getPos().y + 2
	});
	m_pMoneyLabel = new Text("0", rscManager->getFontRsc(16), {
		m_pMoneySpr->getRect()->getPos().x + 17,
		m_pMoneySpr->getRect()->getPos().y + 2
	});

	m_pTimerLabel->setDrawOrder(10);
	m_pHealthLabel->setDrawOrder(10);
	m_pMoneyLabel->setDrawOrder(10);

	m_pTimerLabel->setParentWidget(m_pMainScene);
	m_pHealthLabel->setParentWidget(m_pMainScene);
	m_pMoneyLabel->setParentWidget(m_pMainScene);
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
			//increaseHealth(pNewObj->getLoveFactor());
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

	int newObjId = rand() % m_objectsData->countDataRows();

    char* szName = m_objectsData->getData("Name", newObjId);
	int iPriceMin = intFromStr(m_objectsData->getData("PriceMin", newObjId));
	int iPriceMax = intFromStr(m_objectsData->getData("PriceMax", newObjId));
	int iSprShtFrameId = intFromStr(m_objectsData->getData("SprShtFrameId", newObjId));

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

size2d_t LDGameMode::calculateTruckSize() {
	int iTruckLevel = LDGameManager::get()->getTruckLevel();
	size2d_t sTruckSize = { 2, iTruckLevel };

	return sTruckSize;
}

void LDGameMode::update() {
	/*if (System::get()->getInputSys()->IsKeyPressed(KEYB_G)) {
		giveFreeObjects(1);
	}*/
	
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

	if (m_bWaitForEnd) {
		m_fTimeBeforeEnd -= System::get()->getDeltaTime();
		if (m_fTimeBeforeEnd < 0.) {
			onLevelEndAfterSell();
			m_bWaitForEnd = false;
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
	return m_pTruck->getContent();
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
		if ((pPickedObj = pickObject())) {
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
	for (int i = 0; i < 10; i++) {
		m_pParticleSystem->emitParticle(
			RscManager::get()->getSprShtRsc(18),
			pObj->getRect()->getPos(),
			{
				(float)System::get()->getRandInt(-40, 40),
				(float)System::get()->getRandInt(-40, 40)
			},
			0,
			3,
			5
		);
	}

    increaseHealth(pObj->getLoveFactor()+1);
    
	m_pTruck->doLoadAnim();

	if (!m_pCurrCard) {
		m_bWaitForEnd = true;
		m_fTimeBeforeEnd = 2.;
		//onLevelEndAfterSell();
	}
}

void LDGameMode::onObjSold(Object* pObj) {
	increaseMoney(pObj->getPrice());
	decreaseHealth((pObj->getLoveFactor()+1) * 2);

	if (!m_pCurrCard) {
		m_bWaitForEnd = true;
		m_fTimeBeforeEnd = 2.;
		//onLevelEndAfterSell();
	}

	pObj->setParentWidget(NULL);
	addDataToList(&m_llWidgetTrash, pObj);

	for (int i = 0; i < 10; i++) {
		m_pParticleSystem->emitParticle(
			RscManager::get()->getSprShtRsc(18),
			pObj->getRect()->getPos(),
			{
				(float)System::get()->getRandInt(-40, 40),
				(float)System::get()->getRandInt(-40, 40)
			},
			4,
			7,
			5
		);
	}
}

void LDGameMode::onLevelEndBeforeSell() {
	if (m_llObjectsOwned.size > 0) {
		m_bSellingLeftObj = true;
		m_fTimeBeforeSellNextObj = 0.3;
	}
	else {
		m_fTimeBeforeEnd = 2.;
	}
}

void LDGameMode::onLevelEndAfterSell() {
	if (getHealth() == 0)
		LDGameManager::get()->onLevelFail();
	else 
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
	if (iHealth <= 0) {
		iHealth = 0;
	}

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
	int iStartHealth = START_HEALTH;

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

	*pGameMode = new LDGameMode(pScene, llObjectsOwned, iStartMoney, iStartHealth, iGiveNFreeObjects);
}
