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
#include "progress_bar.hpp"

#include "or_road.hpp"
#include "or_car.hpp"
#include "or_city.hpp"

#include "or_pickup_item.hpp"
#include "or_pickup_items_manager.hpp"



class ORGameMode
{
private:
	static ORGameMode* s_pInstance;

	Scene* m_pMainScene;

	float m_fWinTimer;
    float m_fCarSpeed = 50.0;

	bool m_bWinTimerActivated = false;
	
	int m_iMoney = 0;
    
    float m_fHouse = 0;
    float m_fIndustry = 0;
    float m_fEcology = 0;
    float m_fPopularity = 0;
    float m_fPopulation = 0;
    
    float m_fPopGrowth = 0.0;
    float m_fIndGrowth = 0.0;
    
    // TO DELETE
    float m_fLastFrameIndustry = 0.0;
    float m_fLastFrameHouse = 0.0;
    float m_fLastFramePopulation = 0.0;
    // END TO DELETE

    float m_fTimeBeforeNextBuilding;
    
	float m_fTimer = 30.;
	bool m_bWaitForEnd = false;

	LinkedList m_llWidgetTrash;
    LinkedList m_llPickupList;

	bool m_bLevelEnded = false;
    
    Sprite* m_pBgSpr;
    
    ORRoad m_roadObj;
    ORCar m_carObj;
    ORCity m_cityObj;

	ParticleSystem* m_pParticleSystem;
    
    ProgressBar* m_pIndustryBar;
    ProgressBar* m_pHouseBar;
    ProgressBar* m_pEcologyBar;
    ProgressBar* m_pPopularityBar;
    
    ORPickupItemsManager m_pickupItemsManager;

public:
	ORGameMode(Scene* pMainScene);
	~ORGameMode();

	void initScene();

    void update();
    
    void updateGameStats();
    
    void onHouseItemPicked(ORPickupItem* pPickedUpItem);
    void onIndustryItemPicked(ORPickupItem* pPickedUpItem);
    void onEcologyItemPicked(ORPickupItem* pPickedUpItem);
    void onTomatoItemPicked(ORPickupItem* pPickedUpItem);
    void updateProgressBars();
    
	Scene* getScene();
	float getTimer();
    float getRoadScrollSpeed();
    
    float getIndustryValue();
    float getEcologyValue();
    float getHouseValue();
    float getPopularityValue();
    float getPopulationValue();
    
    float getPopulationGrowth();
    float getIndustryGrowth();

	void increaseMoney(int iMoneyBonus);
	void decreaseMoney(int iMoneyMalus);

	void setMoney(int iMoney);
	void setTimer(float fTimer);
    
    void destroyWidget(IWidget* pWidget);

	void updateMoneyLabel();

	void destroyWidgetsInTrash();

	static ORGameMode* get();

	static void initGameMode(ORGameMode** pGameMode, Scene* pScene);
};


#endif
