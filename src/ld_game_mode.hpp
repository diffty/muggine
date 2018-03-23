#ifndef LD_GAME_MODE_HPP
#define LD_GAME_MODE_HPP

#include <stdio.h>

#include "scene.hpp"
#include "linked_list.hpp"
#include "text.hpp"
#include "csv_reader.hpp"
#include "common_tools.hpp"
#include "platform_tools.hpp"

#include "ld_object_card.hpp"
#include "ld_object.hpp"
#include "ld_truck_content.hpp"
#include "ld_truck.hpp"

#include "particle_system.hpp"


class LDGameMode
{
private:
	static LDGameMode* s_pInstance;

	Scene* m_pMainScene;

	float m_fWinTimer;

	bool m_bWinTimerActivated = false;
	
	int m_iMoney = 0;
	int m_iHealth = 100;
	float m_fTimer = 30.;
	float m_fTimeBeforeSellNextObj = -1;
	float m_fTimeBeforeEnd = -1;
	bool m_bWaitForEnd = false;

	Sprite* m_pBgSpr;
	Truck* m_pTruck;

	LinkedList m_llWidgetTrash;
	LinkedList m_llObjectsOwned;

	CSVReader* m_objectsData;

	ObjectCard* m_pCurrCard = NULL;
	ObjectCard* m_pNextCard = NULL;

	Sprite* m_pTimerSpr;
	Sprite* m_pHealthSpr;
	Sprite* m_pMoneySpr;

	Text* m_pTimerLabel;
	Text* m_pHealthLabel;
	Text* m_pMoneyLabel;

	bool m_bLevelEnded = false;
	bool m_bSellingLeftObj = false;

	ParticleSystem* m_pParticleSystem;


public:
	LDGameMode(Scene* pMainScene, LinkedList pllMachinesOwned, int iStartMoney, int iStartHealth, int iGiveNFreeObjects = 0);
	~LDGameMode();

	void initScene();

	Object* pickObject();
	void giveFreeObjects(int nbObjects);
	void buyObjects();
	Object* generateNewObject();
	ObjectCard* spawnNewCard(Object* pNewObj);
	size2d_t calculateTruckSize();

	void update();

	Scene* getScene();
	TruckContent* getTruckContent();
	int getMoney();
	int getHealth();
	float getTimer();
	LinkedList getObjectsOwnedList();

	void increaseMoney(int iMoneyBonus);
	void decreaseMoney(int iMoneyMalus);

	void increaseHealth(int iHealthBonus);
	void decreaseHealth(int iHealthMalus);

	void setMoney(int iMoney);
	void setHealth(int iHealth);
	void setTimer(float fTimer);

	void updateHealthLabel();
	void updateMoneyLabel();
	void updateTimerLabel();

	void destroyObjectCard(ObjectCard* pObjCard);

	void onObjKeep(Object* pObj);
	void onObjSold(Object* pObj);
	void onLevelEndBeforeSell();
	void onLevelEndAfterSell();
	void onNoMoreCard();

	void destroyWidgetsInTrash();

	static LDGameMode* get();

	static void initGameMode(LDGameMode** pGameMode, Scene* pScene, int iGiveNFreeObjects);
};


#endif
