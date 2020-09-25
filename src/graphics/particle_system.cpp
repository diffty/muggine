#include "particle_system.hpp"

#include "../core/system.hpp"


ParticleSystem::ParticleSystem() {
	initList(&m_llParticles);
	initList(&m_llParticleTrash);
}


ParticleSystem::~ParticleSystem() {
	destroyAllParticles();
}

void ParticleSystem::emitParticle(SpriteSheet* pSprSht, vect2df_t vStartPos, vect2df_t vStartVel, int iStartFrame, int iEndFrame, uint uFPS) {
	ParticleSystem::emitParticle(pSprSht, vStartPos.x, vStartPos.y, vStartVel.x, vStartVel.y, iStartFrame, iEndFrame, uFPS);
}

void ParticleSystem::emitParticle(SpriteSheet* pSprSht, float fStartVectX, float fStartVectY, float fStartVelX, float fStartVelY, int iStartFrame, int iEndFrame, uint uFPS) {
	Particle* pNewParticle = new Particle(pSprSht, fStartVectX, fStartVectY, fStartVelX, fStartVelY, iStartFrame, iEndFrame, uFPS, this);
	addDataToList(&m_llParticles, pNewParticle);
}

void ParticleSystem::update() {
	LLNode* pCurrNode = m_llParticles.pHead;

	while (pCurrNode != NULL) {
		Particle* pCurrParticle = (Particle*)pCurrNode->pData;
		pCurrParticle->update();
		pCurrNode = pCurrNode->pNext;
	}
    
	destroyParticlesInTrash();
	IWidget::update();
}

void ParticleSystem::draw(drawbuffer* pBuffer) {
	LLNode* pCurrNode = m_llParticles.pHead;

	while (pCurrNode != NULL) {
		Particle* pCurrParticle = (Particle*)pCurrNode->pData;
		pCurrParticle->draw(pBuffer);
		pCurrNode = pCurrNode->pNext;
	}
}

void ParticleSystem::destroyParticle(Particle* pParticle) {
	addDataToList(&m_llParticleTrash, pParticle);
}

void ParticleSystem::destroyAllParticles() {
	destroyParticlesInTrash(); // mmh...

	LLNode* pCurrNode = m_llParticles.pHead;

	while (pCurrNode != NULL) {
		Particle* pCurrParticle = (Particle*) pCurrNode->pData;
		destroyParticle(pCurrParticle);
		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llParticles);

	destroyParticlesInTrash();
}

void ParticleSystem::destroyParticlesInTrash() {
	LLNode* pCurrNodeInTrash = m_llParticleTrash.pHead;

	while (pCurrNodeInTrash != NULL) {
		Particle* pCurrParticle = (Particle*)pCurrNodeInTrash->pData;

		LLNode* pNode = searchForNodeInList(&m_llParticles, pCurrParticle);
		if (pNode) {
			removeNodeFromList(&m_llParticles, pNode);
			delete pNode;
		}

		delete pCurrParticle;
		pCurrNodeInTrash = pCurrNodeInTrash->pNext;
	}

	clearList(&m_llParticleTrash);
}

void ParticleSystem::onParticleDie(Particle* pParticle) {
	destroyParticle(pParticle);
}
