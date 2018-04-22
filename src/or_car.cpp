//
//  or_car.cpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_car.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


ORCar::ORCar() :
    m_carSpr(RscManager::get()->getImgRsc(7), {0, 0})
{
    addChildWidget(&m_carSpr);
    m_fShakeYOffset = 1;
    
    m_fTimeBeforeNextShake = SHAKE_TIME;
}

ORCar::~ORCar() {
    
}

void ORCar::update() {
    updateChildren();
    
    vect2df_t vSmokePos;
    vSmokePos.x = getRect()->getPos().x;
    vSmokePos.y = getRect()->getPos().y + 15;
    
    m_particleSystem
        .emitParticle(
                      RscManager::get()->getSprShtRsc(9),
                      vSmokePos,
                      {
                          (float) System::get()->getRandInt(-20, -5),
                          (float) System::get()->getRandInt(-10, 10),
                      },
                      0,
                      2,
                      2
    );
    
    m_fTimeBeforeNextShake -= System::get()->getDeltaTime();
    if (m_fTimeBeforeNextShake < 0.0) {
        translate(0.0, m_fShakeYOffset, TRANSFORM_REL);
        m_fTimeBeforeNextShake = SHAKE_TIME;
        m_fShakeYOffset = - m_fShakeYOffset;
    }
    
    m_particleSystem.update();
}

void ORCar::draw(uint8* buffer) {
    m_particleSystem.draw(buffer);
    drawChildren(buffer);
}
