#include "particle.hpp"
#include "system.hpp"
#include "particle_system.hpp"


Particle::Particle(SpriteSheet* pSprSht, vect2df_t vStartPos, vect2df_t vStartVel, int iStartFrame, int iEndFrame, uint uFPS, ParticleSystem* pParentSystem) :
	AnimatedSprite(pSprSht, vStartPos)
{
	m_vStartPos = vStartPos;
	m_vStartVel = vStartVel;
	m_iStartFrame = iStartFrame;
	m_iEndFrame = iEndFrame;
	m_pParticleSys = pParentSystem;

	setFrame(m_iStartFrame);

	getRect()->setPos(m_vStartPos.x, m_vStartPos.y);

	addState("", iStartFrame, iEndFrame, uFPS, false);
	changeState(0);
}


Particle::~Particle() {

}

void Particle::update() {
	AnimatedSprite::update();

	double dDeltaTime = System::get()->getDeltaTime();

	vect2df_t vCurrPos = getRect()->getPos();

	getRect()->setPos(
		vCurrPos.x + m_vStartVel.x * dDeltaTime,
		vCurrPos.y + m_vStartVel.y * dDeltaTime
	);

	if (getFrame() == m_iEndFrame) {
		m_pParticleSys->onParticleDie(this);
	}
}
