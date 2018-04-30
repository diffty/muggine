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
#include "text.hpp"

#include "particle_system.hpp"
#include "progress_bar.hpp"

#include "or_road.hpp"
#include "or_car.hpp"
#include "or_city.hpp"

#include "or_pickup_item.hpp"
#include "or_pickup_items_manager.hpp"

#include "drawing.hpp"

#include "x_scrolling_img.hpp"
#include "or_intro_animation.hpp"


#define TIME_LIMIT					300
#define CITY_SIZE_SPAWN_LIMIT		300.0
#define RESPAWN_TIME_MIN			5
#define RESPAWN_TIME_MAX			7
#define RESPAWN_FREQ_COEF			4.5
#define POPULATION_GROWTH_FACTOR	20.0
#define MIN_FAILURE_ALLOWED			0.0
#define MAX_FAILURE_ALLOWED			0.3
#define TIME_BETWEEN_STAT_LOSS		3
#define HOUSE_LOSS_FACTOR			2.
#define INDUSTRY_LOSS_FACTOR		1.
#define ECOLOGY_LOSS_FACTOR			2.
#define TOMATO_RECOVER_SPEED		2.
#define TOMATO_MALUS				20.
#define CAR_SPEED					50.0
#define POPULARITY_CHANGE_SPEED     3.0


class ORGameMode
{
private:
	static ORGameMode* s_pInstance;

	Scene* m_pMainScene;

	float m_fWinTimer;
    float m_fCarSpeed;

	bool m_bWinTimerActivated;
	
    float m_fHouse;
    float m_fIndustry;
    float m_fEcology;
    float m_fPopularity;
    float m_fPopularityGoal;
    float m_fPopulation;
    float m_fTomatoMalus;
    
	// TO DELETE
    float m_fPopGrowth;
    float m_fIndGrowth;
    
    float m_fLastFrameIndustry;
    float m_fLastFrameHouse;
    float m_fLastFramePopulation;
    // END TO DELETE

    float m_fTimeBeforeNextBuilding;
    float m_fTimeBeforeNextStatLoss;
    
	float m_fTimer;
	bool m_bWaitForEnd;
	bool m_bLevelEnded;
    bool m_bCarWasMoving;

	LinkedList m_llWidgetTrash;

    Sprite* m_pBgSpr;
    
    ORRoad m_roadObj;
    ORCar m_carObj;
    ORCity m_cityObj;
    
    Sprite* m_pElectionTimerLabel;
    Text* m_pMinutesTimerLabel;
    Text* m_pSecondsTimerLabel;
    Text* m_pSeparatorTimerLabel;
    Text* m_pPopulationCountLabel;
    
    XScrollingImg* m_scrollingMountainsImg;
    XScrollingImg* m_scrollingCloudsImg;

	ParticleSystem* m_pParticleSystem;
    
    ProgressBar* m_pIndustryBar;
    ProgressBar* m_pHouseBar;
    ProgressBar* m_pEcologyBar;
    ProgressBar* m_pPopularityBar;
    
    ORIntroAnimation m_introAnimation;
    
    ORPickupItemsManager m_pickupItemsManager;

public:
	ORGameMode(Scene* pMainScene);
	~ORGameMode();

	void initScene();

    void update();
    void debugDraw(uint8* buffer);
    
    void updateGameStats();
    
    void onHouseItemPicked(ORPickupItem* pPickedUpItem);
    void onIndustryItemPicked(ORPickupItem* pPickedUpItem);
    void onEcologyItemPicked(ORPickupItem* pPickedUpItem);
    void onTomatoItemPicked(ORPickupItem* pPickedUpItem);
    void onTimerEnd();
    
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

	void setTimer(float fTimer);
    void setPopulation(float fPopulation);
    
    void updateTimerLabel();
    
    void destroyWidget(IWidget* pWidget);
	void destroyWidgetsInTrash();

	static ORGameMode* get();

	static void initGameMode(ORGameMode** pGameMode, Scene* pScene);
};


#endif
