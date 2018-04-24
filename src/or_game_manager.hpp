#ifndef LD_GAME_MANAGER_HPP
#define LD_GAME_MANAGER_HPP

#include "input.hpp"
#include "system.hpp"
//#include "sound.hpp"
#include "scene.hpp"
#include "or_game_mode.hpp"
#include "or_main_menu.hpp"
#include "or_level_begin_screen.hpp"
#include "or_level_fail_screen.hpp"
#include "or_level_success_screen.hpp"
#include "or_transition_screen.hpp"
#include "or_gameover_screen.hpp"


enum E_APP_STATE {
	E_APP_STATE_LOGO,
	E_APP_STATE_MENU,
	E_APP_STATE_LEVEL_BEGIN,
	E_APP_STATE_LEVEL_SUCCESS,
	E_APP_STATE_LEVEL_FAIL,
    E_APP_STATE_LEVEL_END,
	E_APP_STATE_INGAME,
	E_APP_STATE_INGAME_MENU,
	E_APP_STATE_NULL
};

enum E_FADE_MODE {
	E_FADE_IN,
	E_FADE_OUT,
};


class ORGameManager
{
private:
	System* m_pSys;
	Input* m_pInputSys;
	ORGameMode* m_pGameMode = NULL;
	ORMainMenu* m_pMainMenu;
	Scene m_gameScene;
	Scene m_menuScene;
	E_APP_STATE m_eCurrState;
	E_APP_STATE m_ePostTransitionState = E_APP_STATE_NULL;

	// Fade shit
	float m_fScreenAlpha = 1;
	float m_fScreenFadeDuration = 0;
	float m_fScreenFadeCurrTime = 0;
	bool m_bDoScreenFade = false;
	E_FADE_MODE m_eFadeMode;

	bool m_bMenuKeyWasPressedLastLoop = false;

	ORLevelBeginScreen* m_pLevelBeginScreen;
	ORLevelFailScreen* m_pLevelFailScreen;
	ORLevelSuccessScreen* m_pLevelSuccessScreen;
	ORTransitionScreen* m_pTransitionScreen;
    ORGameOverScreen* m_pGameOverScreen;

	static ORGameManager* s_pInstance;

public:
	ORGameManager();
	~ORGameManager();

	void onMainMenu();
	void onNewLevel(bool bReplayLevel = false);
	void onLevelSuccess();
	void onLevelFail();
	void onStartLevel();
    void onEndLevel();
	void onEndTransition(ETransitionAnimType eTransType);

	void changeState(E_APP_STATE eNewState, bool bWithTransition = false);
	
	void newGame();
	static void newGameMenuBtnCallback(void* pObj);

	void quitGame();
	static void quitGameMenuBtnCallback(void* pObj);

	static void continueGameMenuBtnCallback(void* pObj);

	void invokeMenu();
	void hideMenu();

	void fadeScreen(E_FADE_MODE eFadeMode, float fDuration);

	Scene* getGameScene();
	Scene* getMenuScene();

	void update();
	void draw(uint8* fb);

	E_APP_STATE getCurrentState();

	static ORGameManager* get();
};


#endif
