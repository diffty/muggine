#ifndef LD_GAME_MODE_HPP
#define LD_GAME_MODE_HPP

#include <stdio.h>

#include "scene.hpp"
#include "linked_list.hpp"
#include "text.hpp"
#include "csv_reader.hpp"
#include "common_tools.hpp"
#include "platform_tools.hpp"
#include "sprite.hpp"
#include "input.hpp"

#include "particle_system.hpp"

#include "or_road.hpp"
#include "or_car.hpp"

#include "or_pickup_item_house.hpp"



class ORGameMode
{
private:
	static ORGameMode* s_pInstance;

	Scene* m_pMainScene;

	float m_fWinTimer;

	bool m_bWinTimerActivated = false;
	
	int m_iMoney = 0;
	int m_iHealth = 100;
	float m_fTimer = 30.;
	bool m_bWaitForEnd = false;

	LinkedList m_llWidgetTrash;

	bool m_bLevelEnded = false;
    
    Sprite m_bgSpr;
    
    ORRoad m_roadObj;
    ORCar m_carObj;

	ParticleSystem* m_pParticleSystem;


public:
	ORGameMode(Scene* pMainScene);
	~ORGameMode();

	void initScene();

    void update();

	Scene* getScene();
	int getMoney();
	float getTimer();

	void increaseMoney(int iMoneyBonus);
	void decreaseMoney(int iMoneyMalus);

	void setMoney(int iMoney);
	void setTimer(float fTimer);

	void updateMoneyLabel();

	void destroyWidgetsInTrash();

	static ORGameMode* get();

	static void initGameMode(ORGameMode** pGameMode, Scene* pScene);
};


#endif
