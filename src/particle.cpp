#include "particle.hpp"
#include "system.hpp"
#include "particle_system.hpp"


Particle::Particle(SpriteSheet* pSprSht, vect2df_t vStartPos, vect2df_t vStartVel, int iStartFrame, int iEndFrame, uint uFPS, ParticleSystem* pParentSystem) :
	AnimatedSprite(pSprSht, vStartPos)
{
	init(pSprSht, vStartPos.x, vStartPos.y, vStartVel.x, vStartVel.y, iStartFrame, iEndFrame, uFPS, pParentSystem);
}

	
	
Particle::Particle(SpriteSheet* pSprSht, float fStartVectX, float fStartVectY, float fStartVelX, float fStartVelY, int iStartFrame, int iEndFrame, uint uFPS, ParticleSystem* pParentSystem) :
	AnimatedSprite(pSprSht, fStartVectX, fStartVectY)
{
	init(pSprSht, fStartVectX, fStartVectY, fStartVelX, fStartVelY, iStartFrame, iEndFrame, uFPS, pParentSystem);
}


Particle::~Particle() {

}

void Particle::init(SpriteSheet* pSprSht, float fStartVectX, float fStartVectY, float fStartVelX, float fStartVelY, int iStartFrame, int iEndFrame, uint uFPS, ParticleSystem* pParentSystem) {
    
    m_vStartPos.x = fStartVectX;
    m_vStartPos.y = fStartVectY;
    m_vStartVel.x = fStartVelX;
    m_vStartVel.y = fStartVelY;
    m_iStartFrame = iStartFrame;
    m_iEndFrame = iEndFrame;
    m_pParticleSys = pParentSystem;
    
    setFrame(m_iStartFrame);
    
    getRect()->setPos(m_vStartPos.x, m_vStartPos.y);
    
    addState("", iStartFrame, iEndFrame, uFPS, false, Particle::onParticleAnimEndCallback, (void*) this);
    changeState(0);
}

void Particle::update() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    AnimatedSprite::update();

	vect2df_t vCurrPos = getRect()->getPos();

	getRect()->setPos(
		vCurrPos.x + m_vStartVel.x * fDeltaTime,
		vCurrPos.y + m_vStartVel.y * fDeltaTime
	);
}

void Particle::onParticleAnimEnd() {
    m_pParticleSys->onParticleDie(this);
}

void Particle::onParticleAnimEndCallback(void* pArg) {
    Particle* pParticle = (Particle*) pArg;
    pParticle->onParticleAnimEnd();
}
