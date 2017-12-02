#include "ld_game_mode.hpp"
#include "ld_game_manager.hpp"
#include "rsc_manager.hpp"


LDGameMode* LDGameMode::s_pInstance = NULL;


LDGameMode::LDGameMode(Scene* pMainScene, int iNbInitSpawns, float fTimeBetweenInitSpawns) {
	LDGameMode::s_pInstance = this;
	
	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;

	initScene();
}

LDGameMode::~LDGameMode() {
	m_pMainScene->destroy();
}

void LDGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
	System* sys = System::get();
}

void LDGameMode::update() {
	if (System::get()->getInputSys()->IsKeyPressed(KEYB_A)) { }
}

Scene* LDGameMode::getScene() {
	return m_pMainScene;
}

LDGameMode* LDGameMode::get() {
	return LDGameMode::s_pInstance;
}

void LDGameMode::initGameMode(LDGameMode** pGameMode, Scene* pScene, int iNbInitSpawns, float fTimeBetweenInitSpawns) {
	if (*pGameMode) delete *pGameMode;
	*pGameMode = new LDGameMode(pScene, iNbInitSpawns, fTimeBetweenInitSpawns);

}