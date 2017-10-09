#ifndef TS_GAME_MODE_HPP
#define TS_GAME_MODE_HPP

#include <stdio.h>

#include "ts_main_character.hpp"
#include "scene.hpp"
#include "linked_list.hpp"

class ThingsManager;

class TSGameMode
{
private:
	float m_fHealth;
	LinkedList m_llCharacters;
	Scene* m_pMainScene;

	ThingsManager* m_pThingsManager;

	static TSGameMode* s_pInstance;


public:
	TSGameMode(Scene* pMainScene, ThingsManager* pThingsManager);
	~TSGameMode();

	void spawnCharacter(vect2df_t vCharPos);

	float getHealth();
	void setHealth(float fHealth);
	void decreaseHealth(float fHealthMalus);
	void increaseHealth(float fHealthBonus);

	void onThingMoved();

	static TSGameMode* get();
};


#endif