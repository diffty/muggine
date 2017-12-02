#include "ts_clone_machine.hpp"



CloneMachine::CloneMachine(SpriteSheet* pSprSht, vect2df_t vPos, int iNbInitSpawns, float fTimeBetweenInitSpawns, float fTimeBeforeFirstSpawn) :
	AnimatedSprite(pSprSht, vPos) {

	addState("idle", 0, 0, 4);
	
	m_iCurrInitSpawnNb = 0;
	m_iNbInitSpawns = iNbInitSpawns;
	m_fTimeBetweenInitSpawns = fTimeBetweenInitSpawns;
	m_fTimeBeforeNextSpawn = fTimeBeforeFirstSpawn;
}


CloneMachine::~CloneMachine() {

}

void CloneMachine::spawnCharacter() {
	vect2df_t vNewCharPos = getRect()->getPos();

	vNewCharPos.x += 3;
	vNewCharPos.y -= 4;

	TSGameMode::get()->spawnMainCharacter(vNewCharPos);
}

void CloneMachine::update() {
	updateChildren();

	if (m_iCurrInitSpawnNb < m_iNbInitSpawns) {
		if (m_fTimeBeforeNextSpawn <= 0) {
			spawnCharacter();
			m_iCurrInitSpawnNb++;
			m_fTimeBeforeNextSpawn = m_fTimeBetweenInitSpawns;
		}

		m_fTimeBeforeNextSpawn -= System::get()->getDeltaTime();
	}
}
