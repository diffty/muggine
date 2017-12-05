#include "ld_game_manager.hpp"


LDGameManager* LDGameManager::s_pInstance = NULL;


LDGameManager::LDGameManager() {
	LDGameManager::s_pInstance = this;

	// Instantiate game mode
	m_pGameMode = NULL;
	
	// Instantiate menu
	m_pMainMenu = new LDMainMenu(&m_menuScene);

	m_pSys = System::get();
	m_pInputSys = System::get()->getInputSys();

	m_pMainMenu->setNewBtnCallback(LDGameManager::newGameMenuBtnCallback, this);
	m_pMainMenu->setQuitBtnCallback(LDGameManager::quitGameMenuBtnCallback, this);
	m_pMainMenu->setContinueBtnCallback(LDGameManager::continueGameMenuBtnCallback, this);

	m_bMenuKeyWasPressedLastLoop = false;

	m_pTransitionScreen = new LDTransitionScreen;

	m_pLevelBeginScreen = NULL;
	m_pLevelFailScreen = NULL;
	m_pLevelSuccessScreen = NULL;

	m_iTruckLevel = 1;

	onMainMenu(); //DEBUG: decommenter ca
	/*// DEBUG
	m_iCurrLevel = 1;
	onStartLevel();
	// --*/
}


LDGameManager::~LDGameManager() {

}

void LDGameManager::newGameMenuBtnCallback(void* pObj) {
	((LDGameManager*)pObj)->newGame();
}

void LDGameManager::quitGameMenuBtnCallback(void* pObj) {
	((LDGameManager*)pObj)->quitGame();
}

void LDGameManager::continueGameMenuBtnCallback(void* pObj) {
	((LDGameManager*)pObj)->hideMenu();
}

void LDGameManager::onMainMenu() {
	m_eCurrState = E_APP_STATE_MENU;
	//Sound::get()->playSound(1);
}

void LDGameManager::onNewLevel(bool bReplayLevel) {
	changeState(E_APP_STATE_LEVEL_BEGIN, true);

	if (!bReplayLevel) {
		m_iCurrLevel++;
	}

	if (m_pLevelBeginScreen) delete m_pLevelBeginScreen;

	m_pLevelBeginScreen = new LDLevelBeginScreen(m_iCurrLevel);

	//fadeScreen(E_FADE_IN, 2);

	//Sound::get()->stopSound(1);
	//Sound::get()->playSound(2);
}

void LDGameManager::onLevelSuccess() {
	changeState(E_APP_STATE_LEVEL_SUCCESS, true);
	if (m_pLevelSuccessScreen) delete m_pLevelSuccessScreen;
	m_pLevelSuccessScreen = new LDLevelSuccessScreen(m_iCurrLevel);
}

void LDGameManager::onLevelFail() {
	changeState(E_APP_STATE_LEVEL_FAIL, true);
	if (m_pLevelFailScreen) delete m_pLevelFailScreen;
	m_pLevelFailScreen = new LDLevelFailScreen();
}

void LDGameManager::onStartLevel() {
	int nbFreeObjects = 0;

	if (m_iCurrLevel <= 1) {
		nbFreeObjects = 3;
	}
	else {
		nbFreeObjects = 1;
	}

	LDGameMode::initGameMode(&m_pGameMode, &m_gameScene, nbFreeObjects);
	changeState(E_APP_STATE_INGAME, true);
}

void LDGameManager::onEndTransition(ETransitionAnimType eTransType) {
	switch (eTransType) {
	case TRANSITION_IN:
		if (m_ePostTransitionState != E_APP_STATE_NULL) {
			m_eCurrState = m_ePostTransitionState;
		}
		m_pTransitionScreen->doTransition(TRANSITION_OUT);
		break;
	}
}

void LDGameManager::onTruckUpgrade() {
	m_iTruckLevel++;
}

void LDGameManager::changeState(E_APP_STATE eNewState, bool bWithTransition) {
	if (bWithTransition) {
		m_ePostTransitionState = eNewState;
		m_pTransitionScreen->doTransition(TRANSITION_IN);
	}
	else {
		m_eCurrState = eNewState;
	}
}

void LDGameManager::newGame() {
	m_iCurrLevel = 0;
	m_iTruckLevel = 1;

	if (m_pGameMode) delete m_pGameMode;
	m_pGameMode = NULL;

	onNewLevel();
}

void LDGameManager::quitGame() {
	m_pSys->quitLoop();
}

void LDGameManager::invokeMenu() {
	m_eCurrState = E_APP_STATE_INGAME_MENU;
	m_pMainMenu->showContinueBtn(true);
}

void LDGameManager::hideMenu() {
	m_eCurrState = E_APP_STATE_INGAME;
}

void LDGameManager::fadeScreen(E_FADE_MODE eFadeMode, float fDuration) {
	m_eFadeMode = eFadeMode;
	m_bDoScreenFade = true;
	m_fScreenFadeDuration = fDuration;
	m_fScreenFadeCurrTime = fDuration;
}

Scene* LDGameManager::getGameScene() {
	return &m_gameScene;
}

Scene* LDGameManager::getMenuScene() {
	return &m_menuScene;
}

int LDGameManager::getTruckLevel() {
	return m_iTruckLevel;
}

void LDGameManager::setTruckLevel(int iTruckLevel) {
	m_iTruckLevel = iTruckLevel;
}


E_APP_STATE LDGameManager::getCurrentState() {
	return m_eCurrState;
}

void LDGameManager::update() {
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

		/*if (m_pSys->getInputSys()->IsKeyPressed(KEYB_E)) {
			onLevelSuccess();
		}*/

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

	/*if (m_pSys->getInputSys()->IsKeyPressed(KEYB_R) || m_pSys->getInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {
		newGame();
	}*/

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
			if (m_pLevelBeginScreen->doMustDisappear() && !m_pTransitionScreen->isActive()) {
				//delete m_pLevelBeginScreen;
				onStartLevel();
			}
			break;

		case E_APP_STATE_LEVEL_SUCCESS:
			m_pLevelSuccessScreen->update();
			if (m_pLevelSuccessScreen->doMustDisappear() && !m_pTransitionScreen->isActive()) {
				//delete m_pLevelSuccessScreen;
				onNewLevel();
			}
			break;

		case E_APP_STATE_LEVEL_FAIL:
			m_pLevelFailScreen->update();
			if (m_pLevelFailScreen->doMustDisappear() && !m_pTransitionScreen->isActive()) {
				//delete m_pLevelFailScreen;
				onMainMenu();
			}
			break;
	}

	m_pTransitionScreen->update();
}

void LDGameManager::draw(uint8* fb) {
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
		//m_gameScene.draw(fb);
		m_pLevelSuccessScreen->draw(fb);
		break;

	case E_APP_STATE_LEVEL_FAIL:
		//m_gameScene.draw(fb);
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

	m_pTransitionScreen->draw(fb);
}

LDGameManager* LDGameManager::get() {
	return LDGameManager::s_pInstance;
}
