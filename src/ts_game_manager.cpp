#include "ts_game_manager.hpp"
#include "ts_things_store.hpp"


TSGameManager* TSGameManager::s_pInstance = NULL;


TSGameManager::TSGameManager() {
	TSGameManager::s_pInstance = this;

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

	m_pLevelBeginScreen = NULL;
	m_pLevelFailScreen = NULL;
	m_pLevelSuccessScreen = NULL;

	onMainMenu();
}


TSGameManager::~TSGameManager() {

}

void TSGameManager::newGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->newGame();
}

void TSGameManager::quitGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->quitGame();
}

void TSGameManager::continueGameMenuBtnCallback(void* pObj) {
	((TSGameManager*)pObj)->hideMenu();
}

void TSGameManager::onMainMenu() {
	m_eCurrState = E_APP_STATE_MENU;

	Sound::get()->playSound(1);
}

void TSGameManager::onNewLevel(bool bReplayLevel) {
	m_eCurrState = E_APP_STATE_LEVEL_BEGIN;
	if (!bReplayLevel) {
		m_iCurrLevel++;
	}
	m_pLevelBeginScreen = new LevelBeginScreen(m_iCurrLevel);

	fadeScreen(E_FADE_IN, 2);

	Sound::get()->stopSound(1);
	Sound::get()->playSound(2);
}

void TSGameManager::onLevelSuccess() {
	m_eCurrState = E_APP_STATE_LEVEL_SUCCESS;
	m_pLevelSuccessScreen = new LevelSuccessScreen(m_iCurrLevel);
	m_iMoney = m_pGameMode->getMoney();
}

void TSGameManager::onLevelFail() {
	m_eCurrState = E_APP_STATE_LEVEL_FAIL;
	m_pLevelFailScreen = new LevelFailScreen();
	
}

void TSGameManager::onStartLevel() {
	TSGameMode::initGameMode(&m_pGameMode, &m_gameScene, m_iCurrLevel, 10);
	m_pGameMode->setMoney(m_iMoney);
	m_eCurrState = E_APP_STATE_INGAME;
}

void TSGameManager::newGame() {
	m_iCurrLevel = 0;
	m_iMoney = 0;
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

void TSGameManager::fadeScreen(E_FADE_MODE eFadeMode, float fDuration) {
	m_eFadeMode = eFadeMode;
	m_bDoScreenFade = true;
	m_fScreenFadeDuration = fDuration;
	m_fScreenFadeCurrTime = fDuration;
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

		if (m_pSys->getInputSys()->IsKeyPressed(KEYB_E)) {
			onLevelSuccess();
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

		case E_APP_STATE_LEVEL_BEGIN:
			m_pLevelBeginScreen->update();
			if (m_pLevelBeginScreen->doMustDisappear()) {
				delete m_pLevelBeginScreen;
				onStartLevel();
			}
			break;

		case E_APP_STATE_LEVEL_SUCCESS:
			m_pLevelSuccessScreen->update();
			if (m_pLevelSuccessScreen->doMustDisappear()) {
				delete m_pLevelSuccessScreen;
				onNewLevel();
			}
			break;

		case E_APP_STATE_LEVEL_FAIL:
			m_gameScene.update();
			m_pLevelFailScreen->update();
			if (m_pLevelFailScreen->doMustDisappear()) {
				delete m_pLevelFailScreen;
				onNewLevel(true);
			}
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

	case E_APP_STATE_LEVEL_BEGIN:
		m_pLevelBeginScreen->draw(fb);
		break;

	case E_APP_STATE_LEVEL_SUCCESS:
		m_gameScene.draw(fb);
		m_pLevelSuccessScreen->draw(fb);
		break;

	case E_APP_STATE_LEVEL_FAIL:
		m_gameScene.draw(fb);
		m_pLevelFailScreen->draw(fb);
		break;
	}

	// Fade shit
	if (m_bDoScreenFade) {
		if (m_fScreenFadeCurrTime <= 0.) {
			m_fScreenFadeCurrTime = 0.;
			m_bDoScreenFade = false;
		}

		m_fScreenAlpha = (m_fScreenFadeCurrTime / m_fScreenFadeDuration);

		switch (m_eFadeMode) {
		case E_FADE_IN:
			m_fScreenAlpha = 1. - m_fScreenAlpha;
			break;

		case E_FADE_OUT:
			m_fScreenAlpha = m_fScreenAlpha;
			break;
		}

		m_fScreenFadeCurrTime -= System::get()->getDeltaTime();
	}
	
	if (m_fScreenAlpha < 1.) {
		int iNbPixels = SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BPP;

		for (int i = 0; i < iNbPixels; i++) {
			fb[i] *= m_fScreenAlpha;
		}
	}
}

TSGameManager* TSGameManager::get() {
	return TSGameManager::s_pInstance;
}
