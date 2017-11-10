#include "ts_game_mode.hpp"
#include "ts_things_manager.hpp"
#include "ts_things_store.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_workguy_thing.hpp"
#include "ts_critical_thing.hpp"
#include "ts_winning_thing.hpp"
#include "ts_main_character.hpp"
#include "ts_win_character.hpp"


TSGameMode* TSGameMode::s_pInstance = NULL;


TSGameMode::TSGameMode(Scene* pMainScene, int iNbInitSpawns, float fTimeBetweenInitSpawns) {
	TSGameMode::s_pInstance = this;
	
	m_pFinalMainCharacter = NULL;
	m_fHealth = 100;
	m_iMoney = 0;
	m_pMainScene = pMainScene;
	m_iNbInitSpawns = iNbInitSpawns;
	m_fTimeBetweenInitSpawns = fTimeBetweenInitSpawns;
	m_fTimeBeforeFirstSpawn = 5;
	
	initList(&m_llCharacters);

	vect2df_t vTextPos;

	vTextPos.x = 10;
	vTextPos.y = 225;
	m_pMoneyLabel = new Text(0, RscManager::get()->getFontRsc(4), vTextPos);
	
	vTextPos.x = 270;
	vTextPos.y = 225;
	m_pHealthLabel = new Text(100, RscManager::get()->getFontRsc(4), vTextPos);

	vTextPos.x = 130;
	vTextPos.y = 225;
	m_pTimerLabel = new Text(0, RscManager::get()->getFontRsc(4), vTextPos);
	m_pTimerLabel->setActive(false);

	m_bWinTimerActivated = false;

	initScene();
}

TSGameMode::~TSGameMode() {
	clearList(&m_llCharacters);

	delete m_pThingsManager;

	m_pMainScene->destroy();
}

void TSGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
	System* sys = System::get();
	
	// Loading CSV Data
	CSVReader objectsData("data/objects.csv");

	// Setting up things
	vect2df_t roomPos;
	roomPos.x = 0.;
	roomPos.y = 0.;

	Sprite* pRoom = new Sprite(rscManager->getImgRsc(2), roomPos);

	vect2df_t vStorePos;
	vStorePos.x = 290;
	vStorePos.y = 0;

	size2df_t sStoreSize;
	sStoreSize.w = 30;
	sStoreSize.h = 180;

	m_pThingsStore = new ThingsStore(vStorePos, sStoreSize, 1, 6);
	m_pThingsManager = new ThingsManager(m_pThingsStore);

	m_pMainScene->addComponent(pRoom);

	vect2df_t testThingPos;
	testThingPos.x = 0;
	testThingPos.y = 0;

	int nbObjects = objectsData.countDataRows();

	vect2df_t vNewThingPos;
	vNewThingPos.x = 0;
	vNewThingPos.y = 0;

	for (int i = 0; i < nbObjects; i++) {
		DraggableThing* newThing = NULL;

		if (strcmp(objectsData.getData("Nice Name", i), "Billet d'Avion") == 0) {
			newThing = (DraggableThing*) new WinningThing(
				rscManager->getSprShtRsc(atoi(objectsData.getData("SprSht", i))),
				atoi(objectsData.getData("FrameNum", i)),
				vNewThingPos,
				m_pThingsManager,
				sys->getInputSys(),
				atoi(objectsData.getData("AppealPower", i)),
				atoi(objectsData.getData("OccupationTime", i)),
				atoi(objectsData.getData("CooldownTime", i)),
				atoi(objectsData.getData("ActionRadius", i)),
				(objectsData.getData("bUsableOnce", i)[0] == 'O') ? true : false,
				(objectsData.getData("bSingleUser", i)[0] == 'O') ? true : false,
				(objectsData.getData("bIsDraggable", i)[0] == 'O') ? true : false,
				(objectsData.getData("bInStore", i)[0] == 'O') ? true : false
			);
		}
		else {
			newThing = (DraggableThing*) new DraggableThing(
				rscManager->getSprShtRsc(atoi(objectsData.getData("SprSht", i))),
				atoi(objectsData.getData("FrameNum", i)),
				vNewThingPos,
				m_pThingsManager,
				sys->getInputSys(),
				atoi(objectsData.getData("AppealPower", i)),
				atoi(objectsData.getData("OccupationTime", i)),
				atoi(objectsData.getData("CooldownTime", i)),
				atoi(objectsData.getData("ActionRadius", i)),
				(objectsData.getData("bUsableOnce", i)[0] == 'O') ? true : false,
				(objectsData.getData("bSingleUser", i)[0] == 'O') ? true : false,
				(objectsData.getData("bIsDraggable", i)[0] == 'O') ? true : false,
				(objectsData.getData("bInStore", i)[0] == 'O') ? true : false
			);
		}

		newThing->setPrice(atoi(objectsData.getData("Price", i)));
		newThing->setUsableOnlyDuringWork((objectsData.getData("bUsableOnlyDuringWork", i)[0] == 'O') ? true : false);
		newThing->setIsWorkThing((objectsData.getData("bIsWorkThing", i)[0] == 'O') ? true : false);
		newThing->setDestroyAfterUse((objectsData.getData("bDestroyAfterUse", i)[0] == 'O') ? true : false);
		newThing->setTitle(objectsData.getData("Name", i));
		newThing->setDesc(objectsData.getData("Description", i));

		m_pThingsStore->addThingToStore(newThing);
	}

	m_pMainScene->addComponent(m_pThingsStore);
	m_pThingsStore->addPageBtnComponentsToMainScene(); // lol

	// Tableaux Nord
	testThingPos.x = 45;
	testThingPos.y = 35;
	CriticalThing* ctN1 = new CriticalThing(rscManager->getSprShtRsc(1), 3, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 100;
	CriticalThing* ctN2 = new CriticalThing(rscManager->getSprShtRsc(1), 3, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 165;
	CriticalThing* ctN3 = new CriticalThing(rscManager->getSprShtRsc(1), 3, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 220;
	CriticalThing* ctN4 = new CriticalThing(rscManager->getSprShtRsc(1), 3, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	// Tableaux Sud
	testThingPos.x = 225;
	testThingPos.y = 170;
	CriticalThing* ctS1 = new CriticalThing(rscManager->getSprShtRsc(1), 5, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 185;
	CriticalThing* ctS2 = new CriticalThing(rscManager->getSprShtRsc(1), 5, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 100;
	CriticalThing* ctS3 = new CriticalThing(rscManager->getSprShtRsc(1), 5, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 50;
	CriticalThing* ctS4 = new CriticalThing(rscManager->getSprShtRsc(1), 5, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	// Tableaux Ouest
	testThingPos.x = 35;
	testThingPos.y = 55;
	CriticalThing* ctO1 = new CriticalThing(rscManager->getSprShtRsc(1), 6, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 110;
	CriticalThing* ctO2 = new CriticalThing(rscManager->getSprShtRsc(1), 6, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 155;
	CriticalThing* ctO3 = new CriticalThing(rscManager->getSprShtRsc(1), 6, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	// Tableaux Est
	testThingPos.x = 235;
	testThingPos.y = 55;
	CriticalThing* ctE1 = new CriticalThing(rscManager->getSprShtRsc(1), 4, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 100;
	CriticalThing* ctE2 = new CriticalThing(rscManager->getSprShtRsc(1), 4, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 155;
	CriticalThing* ctE3 = new CriticalThing(rscManager->getSprShtRsc(1), 4, testThingPos, m_pThingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 165;
	testThingPos.y = 130;
	WorkguyThing* wg1 = new WorkguyThing(rscManager->getSprShtRsc(3), 0, testThingPos, m_pThingsManager, sys->getInputSys(), 1, 5, 90, 75, false, true);

	vect2df_t cloneMachinePos;
	cloneMachinePos.x = 132.;
	cloneMachinePos.y = 95.;

	CloneMachine* cloneMachine = new CloneMachine(rscManager->getSprShtRsc(9), cloneMachinePos, m_iNbInitSpawns, m_fTimeBetweenInitSpawns, m_fTimeBeforeFirstSpawn);

	m_pThingsManager->addThing(ctN1);
	m_pThingsManager->addThing(ctN2);
	m_pThingsManager->addThing(ctN3);
	m_pThingsManager->addThing(ctN4);
	m_pThingsManager->addThing(ctS1);
	m_pThingsManager->addThing(ctS2);
	m_pThingsManager->addThing(ctS3);
	m_pThingsManager->addThing(ctS4);
	m_pThingsManager->addThing(ctO1);
	m_pThingsManager->addThing(ctO2);
	m_pThingsManager->addThing(ctO3);
	m_pThingsManager->addThing(ctE1);
	m_pThingsManager->addThing(ctE2);
	m_pThingsManager->addThing(ctE3);
	m_pThingsManager->addThing(wg1);

	/*m_pMainScene->addComponent(ctN1);
	m_pMainScene->addComponent(ctN2);
	m_pMainScene->addComponent(ctN3);
	m_pMainScene->addComponent(ctN4);
	m_pMainScene->addComponent(ctS1);
	m_pMainScene->addComponent(ctS2);
	m_pMainScene->addComponent(ctS3);
	m_pMainScene->addComponent(ctS4);
	m_pMainScene->addComponent(ctO1);
	m_pMainScene->addComponent(ctO2);
	m_pMainScene->addComponent(ctO3);
	m_pMainScene->addComponent(ctE1);
	m_pMainScene->addComponent(ctE2);
	m_pMainScene->addComponent(ctE3);*/

	m_pMainScene->addComponent(wg1);
	m_pMainScene->addComponent(cloneMachine);

	m_pMainScene->addComponent(m_pMoneyLabel);
	m_pMainScene->addComponent(m_pHealthLabel);
	m_pMainScene->addComponent(m_pTimerLabel);
}

void TSGameMode::spawnMainCharacter(vect2df_t vCharPos) {
	MainCharacter* newChar = new MainCharacter(RscManager::get()->getSprShtRsc(0), vCharPos, m_pThingsManager);

	addDataToList(&m_llCharacters, newChar);
	m_pMainScene->addComponent(newChar);
}

void TSGameMode::spawnWinCharacter(vect2df_t vCharPos) {
	WinCharacter* pNewWinChar = new WinCharacter(RscManager::get()->getSprShtRsc(8), vCharPos);

	addDataToList(&m_llCharacters, pNewWinChar);
	m_pMainScene->addComponent(pNewWinChar);

	m_pWinCharacter = pNewWinChar;
}


float TSGameMode::getHealth() {
	return m_fHealth;
}

void TSGameMode::setHealth(float fHealth) {
	m_fHealth = fHealth;
	char* szNewHealth = Text::intToStr((int) m_fHealth);
	m_pHealthLabel->setText(szNewHealth);
	delete szNewHealth;
}

void TSGameMode::decreaseHealth(float fHealthMalus) {
	setHealth(m_fHealth - fHealthMalus);
}

void TSGameMode::increaseHealth(float fHealthBonus) {
	setHealth(m_fHealth + fHealthBonus);
}


int TSGameMode::getMoney() {
	return m_iMoney;
}

void TSGameMode::setMoney(int iMoney) {
	m_iMoney = iMoney;
	char* szNewMoney = Text::intToStr(iMoney);
	m_pMoneyLabel->setText(szNewMoney);
	delete szNewMoney;
}

void TSGameMode::decreaseMoney(int iMoneyMalus) {
	setMoney(m_iMoney - iMoneyMalus);
}

void TSGameMode::increaseMoney(int iMoneyBonus) {
	setMoney(m_iMoney + iMoneyBonus);
}


void TSGameMode::onThingMoved() {
	LLNode* pCurrNode = m_llCharacters.pHead;

	while (pCurrNode != NULL) {
		MainCharacter* pCurrCharacter = (MainCharacter*) pCurrNode->pData;
		
		pCurrCharacter->onThingMoved();

		pCurrNode = pCurrNode->pNext;
	}
}

void TSGameMode::launchWinTimer() {
	if (!m_bWinTimerActivated) {
		m_fWinTimer = 10;
		m_bWinTimerActivated = true;
		m_pTimerLabel->setActive(true);
	}

	m_iNbActiveWinThings++;
	printf("%i\n", m_iNbActiveWinThings);
}

void TSGameMode::stopWinTimer() {
	if (--m_iNbActiveWinThings == 0) {
		printf("%i\n", m_iNbActiveWinThings);
		m_bWinTimerActivated = false;
		m_pTimerLabel->setActive(false);
	}
}

void TSGameMode::onWinTimerEnd() {
	vect2df_t charPos;
	charPos.x = 135;
	charPos.y = 185;

	spawnWinCharacter(charPos);

	LLNode* pCurrNode = m_llCharacters.pHead;

	int i = 0;

	while (pCurrNode != NULL) {
		AnimatedSprite* pCurrChar = (AnimatedSprite*)pCurrNode->pData;

		if (pCurrChar->getClassType()->getClassTypeName() == "MainCharacter") {
			if (i == 0) {
				((MainCharacter*) pCurrChar)->onWinWalk();
				m_pFinalMainCharacter = ((MainCharacter*)pCurrChar);
			}
			else {
				((MainCharacter*) pCurrChar)->onWinVanish();
			}
		}

		i++;

		pCurrNode = pCurrNode->pNext;
	}
}

MainCharacter* TSGameMode::getFinalMainCharacter() {
	return m_pFinalMainCharacter;
}

WinCharacter* TSGameMode::getWinCharacter() {
	return m_pWinCharacter;
}

WinCharacter* TSGameMode::findWinCharacter() {
	LLNode* pCurrNode = m_llCharacters.pHead;

	int i = 0;

	while (pCurrNode != NULL) {
		AnimatedSprite* pCurrChar = (AnimatedSprite*) pCurrNode->pData;

		if (pCurrChar->getClassType()->getClassTypeName() == "WinCharacter")
			return (WinCharacter*) pCurrChar;

		pCurrNode = pCurrNode->pNext;
	}
}


void TSGameMode::destroyCharacters() {
	LLNode* pCurrNode = m_llCharacters.pHead;

	while (pCurrNode != NULL) {
		AnimatedSprite* pCurrChar = (AnimatedSprite*)pCurrNode->pData;

		m_pMainScene->removeComponent(pCurrChar);

		if (pCurrChar->getClassType()->getClassTypeName() == "WinCharacter") {
			delete ((WinCharacter*) pCurrChar);
		}
		else {
			delete ((MainCharacter*) pCurrChar);
		}

		pCurrNode = pCurrNode->pNext;
	}
}

void TSGameMode::update() {
	m_pThingsManager->processThingsToDeleteList();

	if (m_bWinTimerActivated) {
		m_fWinTimer -= System::get()->getDeltaTime();

		char* szNewTime = Text::intToStr(m_fWinTimer);
		m_pTimerLabel->setText(szNewTime);
		delete szNewTime;

		if (m_fWinTimer <= 0) {
			onWinTimerEnd();
			m_bWinTimerActivated = false;
		}
	}
}

ThingsStore* TSGameMode::getThingsStore() {
	return m_pThingsStore;
}

ThingsManager* TSGameMode::getThingsManager() {
	return m_pThingsManager;
}

Scene* TSGameMode::getScene() {
	return m_pMainScene;
}

TSGameMode* TSGameMode::get() {
	/*if (TSGameMode::s_pInstance == NULL) {
		return new TSGameMode;
	}*/
	
	return TSGameMode::s_pInstance;
}

void TSGameMode::initGameMode(TSGameMode** pGameMode, Scene* pScene, int iNbInitSpawns, float fTimeBetweenInitSpawns) {
	if (*pGameMode) delete *pGameMode;
	*pGameMode = new TSGameMode(pScene, iNbInitSpawns, fTimeBetweenInitSpawns);

}
