#ifndef LD_GAME_MODE_HPP
#define LD_GAME_MODE_HPP

#include <stdio.h>

#include "scene.hpp"
#include "linked_list.hpp"
#include "text.hpp"
#include "csv_reader.hpp"



class LDGameMode
{
private:
	Scene* m_pMainScene;

	float m_fWinTimer;

	bool m_bWinTimerActivated = false;
	
	static LDGameMode* s_pInstance;


public:
	LDGameMode(Scene* pMainScene, int iNbInitSpawns, float fTimeBetweenInitSpawns);
	~LDGameMode();

	void initScene();

	void update();

	Scene* getScene();

	static LDGameMode* get();

	static void initGameMode(LDGameMode** pGameMode, Scene* pScene, int iNbInitSpawns, float fTimeBetweenInitSpawns);
};


#endif
