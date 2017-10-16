#include "ts_game_mode.hpp"

#include "ts_things_manager.hpp"

TSGameMode* TSGameMode::s_pInstance = NULL;


TSGameMode::TSGameMode(Scene* pMainScene, ThingsManager* pThingsManager) {
	TSGameMode::s_pInstance = this;
	
	m_fHealth = 100;
	m_iMoney = 0;
	m_pMainScene = pMainScene;
	m_pThingsManager = pThingsManager;
	
	initList(&m_llCharacters); // TODO: libérer la mémoire

	vect2df_t vTextPos;

	vTextPos.x = 10;
	vTextPos.y = 225;
	m_pMoneyLabel = new Text(Text::intToStr(0), RscManager::get()->getFontRsc(4), vTextPos);
	
	vTextPos.x = 270;
	vTextPos.y = 225;
	m_pHealthLabel = new Text(Text::intToStr(100), RscManager::get()->getFontRsc(4), vTextPos);

	m_pMainScene->addComponent(m_pMoneyLabel);
	m_pMainScene->addComponent(m_pHealthLabel);
}

TSGameMode::~TSGameMode() {
	delete m_pMoneyLabel;
	delete m_pHealthLabel;
}

void TSGameMode::spawnCharacter(vect2df_t vCharPos) {
	MainCharacter* newChar = new MainCharacter(RscManager::get()->getSprShtRsc(0), vCharPos, m_pThingsManager);
    
	addDataToList(&m_llCharacters, newChar);
	m_pMainScene->addComponent(newChar);
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


TSGameMode* TSGameMode::get() {
	/*if (TSGameMode::s_pInstance == NULL) {
		return new TSGameMode;
	}*/
	
	return TSGameMode::s_pInstance;
}
