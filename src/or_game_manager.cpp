#include "or_game_manager.hpp"


ORGameManager* ORGameManager::s_pInstance = NULL;


ORGameManager::ORGameManager() {
	ORGameManager::s_pInstance = this;

    m_pLevelBeginScreen = NULL;
    m_pLevelFailScreen = NULL;
    m_pLevelSuccessScreen = NULL;
    m_pTransitionScreen = NULL;
    m_pGameOverScreen = NULL;

	// Instantiate game mode
	m_pGameMode = NULL;
	
	// Instantiate menu
	m_pMainMenu = new ORMainMenu(&m_menuScene);

	m_pSys = System::get();
	m_pInputSys = System::get()->getInputSys();

	/*m_pMainMenu->setNewBtnCallback(ORGameManager::newGameMenuBtnCallback, this);
	m_pMainMenu->setQuitBtnCallback(ORGameManager::quitGameMenuBtnCallback, this);
	m_pMainMenu->setContinueBtnCallback(ORGameManager::continueGameMenuBtnCallback, this);*/

	m_bMenuKeyWasPressedLastLoop = false;

	m_fScreenAlpha = 1;
	m_fScreenFadeDuration = 0;
	m_fScreenFadeCurrTime = 0;
	m_bDoScreenFade = false;

	m_ePostTransitionState = E_APP_STATE_NULL;

	// onMainMenu(); // DEBUG: decommenter ca
    
	onStartLevel();
}


ORGameManager::~ORGameManager() {

}

void ORGameManager::newGameMenuBtnCallback(void* pObj) {
	((ORGameManager*)pObj)->newGame();
}

void ORGameManager::quitGameMenuBtnCallback(void* pObj) {
	((ORGameManager*)pObj)->quitGame();
}

void ORGameManager::continueGameMenuBtnCallback(void* pObj) {
	((ORGameManager*)pObj)->hideMenu();
}

void ORGameManager::onMainMenu() {
	m_eCurrState = E_APP_STATE_MENU;
	//Sound::get()->playSound(1);
}

void ORGameManager::onNewLevel(bool bReplayLevel) {
	changeState(E_APP_STATE_LEVEL_BEGIN, true);

	if (m_pLevelBeginScreen) delete m_pLevelBeginScreen;

	m_pLevelBeginScreen = new ORLevelBeginScreen(0);

	//fadeScreen(E_FADE_IN, 2);

	//Sound::get()->stopSound(1);
	//Sound::get()->playSound(2);
}

void ORGameManager::onLevelSuccess() {
	changeState(E_APP_STATE_LEVEL_SUCCESS, true);
	if (m_pLevelSuccessScreen) delete m_pLevelSuccessScreen;
	m_pLevelSuccessScreen = new ORLevelSuccessScreen(0);
}

void ORGameManager::onLevelFail() {
	changeState(E_APP_STATE_LEVEL_FAIL, true);
	if (m_pLevelFailScreen) delete m_pLevelFailScreen;
	m_pLevelFailScreen = new ORLevelFailScreen();
}

void ORGameManager::onStartLevel() {
	ORGameMode::initGameMode(&m_pGameMode, &m_gameScene);
	changeState(E_APP_STATE_INGAME, false);
}

void ORGameManager::onEndLevel() {
    m_pGameOverScreen = new ORGameOverScreen();
    changeState(E_APP_STATE_LEVEL_END, false);
}

void ORGameManager::onEndTransition(ETransitionAnimType eTransType) {
	switch (eTransType) {
	case TRANSITION_IN:
		if (m_ePostTransitionState != E_APP_STATE_NULL) {
			m_eCurrState = m_ePostTransitionState;
		}
		m_pTransitionScreen->doTransition(TRANSITION_OUT);
		break;
        
    case TRANSITION_OUT:
        break;
	}
}

void ORGameManager::changeState(E_APP_STATE eNewState, bool bWithTransition) {
	if (bWithTransition) {
		m_ePostTransitionState = eNewState;
		//m_pTransitionScreen->doTransition(TRANSITION_IN);
	}
	else {
		m_eCurrState = eNewState;
	}
}

void ORGameManager::newGame() {
	if (m_pGameMode) delete m_pGameMode;
	m_pGameMode = NULL;

	onNewLevel();
}

void ORGameManager::quitGame() {
	m_pSys->quitLoop();
}

void ORGameManager::invokeMenu() {
	m_eCurrState = E_APP_STATE_INGAME_MENU;
	//m_pMainMenu->showContinueBtn(true);
}

void ORGameManager::hideMenu() {
	m_eCurrState = E_APP_STATE_INGAME;
}

void ORGameManager::fadeScreen(E_FADE_MODE eFadeMode, float fDuration) {
	m_eFadeMode = eFadeMode;
	m_bDoScreenFade = true;
	m_fScreenFadeDuration = fDuration;
	m_fScreenFadeCurrTime = fDuration;
}

Scene* ORGameManager::getGameScene() {
	return &m_gameScene;
}

Scene* ORGameManager::getMenuScene() {
	return &m_menuScene;
}


E_APP_STATE ORGameManager::getCurrentState() {
	return m_eCurrState;
}

void ORGameManager::update() {
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
            onStartLevel();
			break;

		case E_APP_STATE_LEVEL_SUCCESS:
			m_pLevelSuccessScreen->update();
			onNewLevel();
			break;

		case E_APP_STATE_LEVEL_FAIL:
			m_pLevelFailScreen->update();
            onMainMenu();
			break;
            
        case E_APP_STATE_LEVEL_END:
            m_pGameOverScreen->update();
            break;
        
        case E_APP_STATE_NULL:
            break;
	}
}

void ORGameManager::draw(uint8* fb) {
	switch (m_eCurrState) {
	case E_APP_STATE_LOGO:
		break;

	case E_APP_STATE_INGAME:
		m_gameScene.draw(fb);
        m_pGameMode->debugDraw(fb);
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
            
    case E_APP_STATE_LEVEL_END:
        m_gameScene.draw(fb);
        m_pGameOverScreen->draw(fb);
        break;
    
    case E_APP_STATE_NULL:
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

ORGameManager* ORGameManager::get() {
	return ORGameManager::s_pInstance;
}
