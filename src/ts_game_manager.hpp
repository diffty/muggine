#ifndef TS_GAME_MANAGER_HPP
#define TS_GAME_MANAGER_HPP

#include "input.hpp"
#include "system.hpp"
#include "scene.hpp"
#include "ts_game_mode.hpp"
#include "ts_main_menu.hpp"


enum E_APP_STATE {
	E_APP_STATE_LOGO,
	E_APP_STATE_MENU,
	E_APP_STATE_LEVEL_INTRO,
	E_APP_STATE_LEVEL_SUCCESS,
	E_APP_STATE_LEVEL_FAIL,
	E_APP_STATE_INGAME,
	E_APP_STATE_INGAME_MENU,
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
	int m_iCurrLevel = 1;

	bool m_bMenuKeyWasPressedLastLoop = false;


public:
	TSGameManager();
	~TSGameManager();

	void onNewLevel();
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

	Scene* getGameScene();
	Scene* getMenuScene();

	void update();
	void draw(uint8* fb);

	E_APP_STATE getCurrentState();

	
};


#endif
