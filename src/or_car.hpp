//
//  or_car.hpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_car_hpp
#define or_car_hpp

#include <stdio.h>

#include "iwidget.hpp"
#include "sprite.hpp"
#include "particle_system.hpp"
#include "text_bubble.hpp"

#define SHAKE_TIME 0.1
#define ADVICE_SHOW_TIME 5.0
#define TIME_BETWEEN_PARTICLES_NORMAL 0.3
#define TIME_BETWEEN_PARTICLES_ACCELERATE 0.08
#define TIME_BETWEEN_PARTICLES_BREAK 1


enum ECarState {
    ECARSTATE_NORMAL,
    ECARSTATE_ACCELERATE,
    ECARSTATE_BREAK
};


class ORCar : public IWidget {
private:
    Sprite m_carSpr;
    Sprite m_junoHead;
    TextBubble m_junoAdviceBubble;
    
    Rectf m_collisionRect;
    
    ParticleSystem m_smokeParticleSystem;
    ParticleSystem m_trailParticleSystem;
    
    int m_fShakeYOffset;
    float m_fTimeBeforeNextShake;
    float m_fTimeBeforeAdviceDismiss;
    float m_fTimeBeforeNextSmokeParticle;
    
    float m_fTimeBetweenSmokeParticles;
    
    ECarState m_eState;

public:
    ORCar();
    ~ORCar();
    
    void updateCollisionRect();
    Rectf* getCollisionRect();
    
    ECarState getState();
    
    void update();
    void draw(uint8*);
    
    void onNormal();
    void onAccelerating();
    void onBreaking();
    
    void translate(float x, float y, ETransformMode transformMode = TRANSFORM_REL);
    
    void showAdvice(char* szText);
    void hideAdvice();
};


#endif /* or_car_hpp */
