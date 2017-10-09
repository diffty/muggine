#include "ts_game_mode.hpp"

#include "ts_things_manager.hpp"

TSGameMode* TSGameMode::s_pInstance = NULL;


TSGameMode::TSGameMode(Scene* pMainScene, ThingsManager* pThingsManager) {
	TSGameMode::s_pInstance = this;
	
	m_fHealth = 100;
	m_pMainScene = pMainScene;
	m_pThingsManager = pThingsManager;
	
	initList(&m_llCharacters); // TODO: libérer la mémoire
}


TSGameMode::~TSGameMode() {
	
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
}

void TSGameMode::decreaseHealth(float fHealthMalus) {
	setHealth(m_fHealth - fHealthMalus);
}

void TSGameMode::increaseHealth(float fHealthBonus) {
	setHealth(m_fHealth + fHealthBonus);
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
