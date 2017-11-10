#include "ts_game_manager.hpp"
#include "ts_things_store.hpp"


TSGameManager::TSGameManager() {
	m_eCurrState = E_APP_STATE_MENU;

	// Instantiate game mode
	m_pGameMode = NULL;
	
	// Instantiate menu
	m_pMainMenu = new TSMainMenu(&m_menuScene);

	m_pSys = System::get();
	m_pInputSys = System::get()->getInputSys();

	m_pMainMenu->setNewBtnCallback(TSGameManager::newGameMenuBtnCallback, this);
	m_pMainMenu->setQuitBtnCallback(TSGameManager::quitGameMenuBtnCallback, this);
	m_pMainMenu->setContinueBtnCallback(TSGameManager::continueGameMenuBtnCallback, this);

	m_bMenuKeyWasPressedLastLoop = false;
}


TSGameManager::~TSGameManager() {

}

void TSGameManager::newGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->onStartLevel();
}

void TSGameManager::quitGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->quitGame();
}

void TSGameManager::continueGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->hideMenu();
}

void TSGameManager::onNewLevel() {
	m_eCurrState = E_APP_STATE_LEVEL_INTRO;
}

void TSGameManager::onLevelSuccess() {
	m_eCurrState = E_APP_STATE_LEVEL_SUCCESS;
}

void TSGameManager::onLevelFail() {
	m_eCurrState = E_APP_STATE_LEVEL_FAIL;
}

void TSGameManager::onStartLevel() {
	TSGameMode::initGameMode(&m_pGameMode, &m_gameScene, 1, 10);
	m_eCurrState = E_APP_STATE_INGAME;
}

void TSGameManager::newGame() {
	m_iCurrLevel = 1;
	onNewLevel();
}

void TSGameManager::quitGame() {
	m_pSys->quitLoop();
}

void TSGameManager::invokeMenu() {
	m_eCurrState = E_APP_STATE_INGAME_MENU;
	m_pMainMenu->showContinueBtn(true);
}

void TSGameManager::hideMenu() {
	m_eCurrState = E_APP_STATE_INGAME;
}

Scene* TSGameManager::getGameScene() {
	return &m_gameScene;
}

Scene* TSGameManager::getMenuScene() {
	return &m_menuScene;
}

E_APP_STATE TSGameManager::getCurrentState() {
	return m_eCurrState;
}

void TSGameManager::update() {
	// Scan all the inputs. This should be done once for each frame
	vect2d_t touchPt;
	if (m_pSys->getInputSys()->GetTouch(&touchPt)) {
		m_gameScene.receiveTouchInput(touchPt);
	}

	MouseEvent* mouseEvt = m_pSys->getInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);
	
	if (m_eCurrState == E_APP_STATE_INGAME) {
		if (mouseEvt) {
			m_gameScene.receiveTouchInput(mouseEvt->position);
		}

		m_pGameMode->update();
	}
	else if (m_eCurrState == E_APP_STATE_MENU || m_eCurrState == E_APP_STATE_INGAME_MENU) {
		if (mouseEvt) {
			m_menuScene.receiveTouchInput(mouseEvt->position);
		}
	}
	
	if (m_pSys->getInputSys()->IsKeyPressed(KEYB_ESCAPE) || m_pSys->getInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {

		if (!m_bMenuKeyWasPressedLastLoop) {
			if (m_eCurrState == E_APP_STATE_INGAME)
				invokeMenu();
			else if (m_eCurrState == E_APP_STATE_INGAME_MENU)
				hideMenu();

			m_bMenuKeyWasPressedLastLoop = true;
		}
	}
	else {
		m_bMenuKeyWasPressedLastLoop = false;
	}

	if (m_pSys->getInputSys()->IsKeyPressed(KEYB_R) || m_pSys->getInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {
		newGame();
	}

	switch (m_eCurrState) {
		case E_APP_STATE_LOGO:
			break;

		case E_APP_STATE_INGAME:
			m_gameScene.update();
			break;

		case E_APP_STATE_INGAME_MENU:
			m_menuScene.update();
			break;

		case E_APP_STATE_MENU:
			m_menuScene.update();
			break;
	}
}

void TSGameManager::draw(uint8* fb) {
	switch (m_eCurrState) {
	case E_APP_STATE_LOGO:
		break;

	case E_APP_STATE_INGAME:
		m_gameScene.draw(fb);
		break;

	case E_APP_STATE_INGAME_MENU:
		m_gameScene.draw(fb);
		m_menuScene.draw(fb);
		break;

	case E_APP_STATE_MENU:
		m_menuScene.draw(fb);
		break;
	}

	// Fade in/Fade out code HERE
}