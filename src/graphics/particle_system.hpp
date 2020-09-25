#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "../utils/linked_list.hpp"
#include "particle.hpp"
#include "../core/iwidget.hpp"


class ParticleSystem : public IWidget
{
private:
	LinkedList m_llParticles;
	LinkedList m_llParticleTrash;

public:
	ParticleSystem();
	~ParticleSystem();

	void emitParticle(SpriteSheet* pSprSht, vect2df_t vStartVect, vect2df_t vStartVel, int startFrame, int endFrame, uint uFPS);
	void emitParticle(SpriteSheet* pSprSht, float fStartVectX, float fStartVectY, float fStartVelX, float fStartVelY, int startFrame, int endFrame, uint uFPS);

	void update();
	void draw(drawbuffer* pBuffer);
	
	void destroyParticle(Particle* pParticle);
	void destroyAllParticles();
	void destroyParticlesInTrash();
	
	void onParticleDie(Particle* pParticle);

};


#endif
