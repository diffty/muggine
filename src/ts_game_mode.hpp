#ifndef TS_GAME_MODE_HPP
#define TS_GAME_MODE_HPP

#include <stdio.h>

#include "scene.hpp"
#include "linked_list.hpp"
#include "text.hpp"
#include "ts_clone_machine.hpp"
#include "csv_reader.hpp"


class ThingsManager;
class ThingsStore;

class DraggableThing;
class WinningThing;
class CriticalThing;
class WorkguyThing;

class MainCharacter;
class WinCharacter;


class TSGameMode
{
private:
	float m_fHealth;
	int m_iMoney;
	int m_iNbActiveWinThings = 0;
	int m_iNbInitSpawns;
	float m_fTimeBetweenInitSpawns;
	float m_fTimeBeforeFirstSpawn;

	LinkedList m_llCharacters;
	Text* m_pMoneyLabel;
	Text* m_pHealthLabel;
	Text* m_pTimerLabel;
	Sprite* m_pTimerSprite;
	AnimatedSprite* m_pArtistSpr;

	MainCharacter* m_pFinalMainCharacter;
	WinCharacter* m_pWinCharacter;

	ThingsManager* m_pThingsManager;
	ThingsStore* m_pThingsStore;
	Scene* m_pMainScene;

	float m_fWinTimer;

	bool m_bWinTimerActivated = false;
	
	static TSGameMode* s_pInstance;


public:
	TSGameMode(Scene* pMainScene, int iNbInitSpawns, float fTimeBetweenInitSpawns);
	~TSGameMode();

	void initScene();

	void spawnMainCharacter(vect2df_t vCharPos);
	void spawnWinCharacter(vect2df_t vCharPos);

	float getHealth();
	void setHealth(float fHealth);
	void decreaseHealth(float fHealthMalus);
	void increaseHealth(float fHealthBonus);

	int getMoney();
	void setMoney(int iMoney);
	void increaseMoney(int iMoneyMalus);
	void decreaseMoney(int iMoneyBonus);

	void onThingMoved();
	void onHealthUpdate();

	void launchWinTimer();
	void stopWinTimer();

	void onWinTimerEnd();

	MainCharacter* getFinalMainCharacter();
	WinCharacter* getWinCharacter();
	WinCharacter* findWinCharacter();

	void destroyCharacters();

	void update();

	ThingsStore* getThingsStore();
	ThingsManager* getThingsManager();
	Scene* getScene();

	static TSGameMode* get();
	static void initGameMode(TSGameMode** pGameMode, Scene* pScene, int iNbInitSpawns, float fTimeBetweenInitSpawns);
};


#endif