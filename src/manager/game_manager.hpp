#ifndef LD_GAME_MANAGER_HPP
#define LD_GAME_MANAGER_HPP

#include "../core/input.hpp"
#include "../core/system.hpp"
//#include "core/sound.hpp"
#include "../graphics/scene.hpp"
#include "../game/game_mode.hpp"
#include "../game/xx_main_menu.hpp"
#include "../game/xx_level_begin_screen.hpp"
#include "../game/xx_level_fail_screen.hpp"
#include "../game/xx_level_success_screen.hpp"
#include "../game/xx_gameover_screen.hpp"
#include "../graphics/transition_screen.hpp"


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


class GameManager
{
private:
	System* m_pSys;
	Input* m_pInputSys;
	GameMode* m_pGameMode;
	XXMainMenu* m_pMainMenu;
	Scene m_gameScene;
	Scene m_menuScene;
	E_APP_STATE m_eCurrState;
	E_APP_STATE m_ePostTransitionState;

	// Fade shit
	float m_fScreenAlpha;
	float m_fScreenFadeDuration;
	float m_fScreenFadeCurrTime;
	bool m_bDoScreenFade;
	E_FADE_MODE m_eFadeMode;

	bool m_bMenuKeyWasPressedLastLoop;

	XXLevelBeginScreen* m_pLevelBeginScreen;
	XXLevelFailScreen* m_pLevelFailScreen;
	XXLevelSuccessScreen* m_pLevelSuccessScreen;
	TransitionScreen* m_pTransitionScreen;
    XXGameOverScreen* m_pGameOverScreen;

	static GameManager* s_pInstance;

public:
	GameManager();
	~GameManager();

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

	static GameManager* get();
};


#endif
