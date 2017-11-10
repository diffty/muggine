#ifndef TS_GAME_MANAGER_HPP
#define TS_GAME_MANAGER_HPP

#include "input.hpp"
#include "system.hpp"
#include "sound.hpp"
#include "scene.hpp"
#include "ts_game_mode.hpp"
#include "ts_main_menu.hpp"
#include "ts_level_begin_screen.hpp"
#include "ts_level_fail_screen.hpp"
#include "ts_level_success_screen.hpp"


enum E_APP_STATE {
	E_APP_STATE_LOGO,
	E_APP_STATE_MENU,
	E_APP_STATE_LEVEL_BEGIN,
	E_APP_STATE_LEVEL_SUCCESS,
	E_APP_STATE_LEVEL_FAIL,
	E_APP_STATE_INGAME,
	E_APP_STATE_INGAME_MENU,
};

enum E_FADE_MODE {
	E_FADE_IN,
	E_FADE_OUT,
};


class TSGameManager
{
private:
	System* m_pSys;
	Input* m_pInputSys;
	TSGameMode* m_pGameMode = NULL;
	TSMainMenu* m_pMainMenu;
	Scene m_gameScene;
	Scene m_menuScene;
	E_APP_STATE m_eCurrState;
	int m_iMoney = 0;
	int m_iCurrLevel = 0;

	// Fade shit
	float m_fScreenAlpha = 1;
	float m_fScreenFadeDuration = 0;
	float m_fScreenFadeCurrTime = 0;
	bool m_bDoScreenFade = false;
	E_FADE_MODE m_eFadeMode;

	bool m_bMenuKeyWasPressedLastLoop = false;

	LevelBeginScreen* m_pLevelBeginScreen;
	LevelFailScreen* m_pLevelFailScreen;
	LevelSuccessScreen* m_pLevelSuccessScreen;

	static TSGameManager* s_pInstance;

public:
	TSGameManager();
	~TSGameManager();

	void onMainMenu();
	void onNewLevel(bool bReplayLevel = false);
	void onLevelSuccess();
	void onLevelFail();
	void onStartLevel();
	
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

	static TSGameManager* get();
};


#endif
