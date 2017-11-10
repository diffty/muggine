#ifndef TS_CLONE_MACHINE_HPP
#define TS_CLONE_MACHINE_HPP

#include "animatedsprite.hpp"
#include "ts_game_mode.hpp"


class CloneMachine : public AnimatedSprite
{
private:
	int m_iNbInitSpawns;
	int m_iCurrInitSpawnNb;
	float m_fTimeBetweenInitSpawns;
	float m_fTimeBeforeNextSpawn;

public:
	CloneMachine(SpriteSheet* pSprSht, vect2df_t vPos, int iNbInitSpawns, float fTimeBetweenFirstSpawns, float fTimeBeforeFirstSpawn);
	~CloneMachine();

	void spawnCharacter();

	void update();
};

#endif