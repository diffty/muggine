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
    m_carSpr(RscManager::get()->getImgRsc(7), {0, 0}),
    m_junoHead(RscManager::get()->getImgRsc(16), {0, 0}),
    m_junoAdviceBubble("MIAW", RscManager::get()->getFontRsc(2), 0, 0, 50, 10)
{
    addChildWidget(&m_carSpr);
    addChildWidget(&m_junoHead);
    addChildWidget(&m_junoAdviceBubble);

    m_junoHead.translate(
                           m_rect.getPos().x - m_junoHead.getRect()->getSize().w + 12,
                           m_rect.getPos().y - m_junoHead.getRect()->getSize().h + 5,
                           TRANSFORM_ABS
                           );
    
    m_junoAdviceBubble.updateSize();
    
    m_junoAdviceBubble.translate(
                                 m_junoHead.getRect()->getPos().x,
                                 m_rect.getPos().y - m_junoHead.getRect()->getSize().h - m_junoAdviceBubble.getRect()->getSize().h - 1,
                                 TRANSFORM_ABS
                                 );
    
    m_fShakeYOffset = 1;
    
    m_fTimeBeforeNextShake = SHAKE_TIME;
    
    getRect()->setSize(m_carSpr.getRect()->getSize().w, m_carSpr.getRect()->getSize().h);
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
    
    m_fTimeBeforeAdviceDismiss -= System::get()->getDeltaTime();
    if (m_fTimeBeforeAdviceDismiss < 0.0) {
        hideAdvice();
    }
    
    m_particleSystem.update();
}

void ORCar::draw(uint8* buffer) {
    m_particleSystem.draw(buffer);
    drawChildren(buffer);
}

void ORCar::showAdvice(char* szText) {
    m_junoAdviceBubble.setText(szText);
    m_junoAdviceBubble.updateSize();
    m_fTimeBeforeAdviceDismiss = ADVICE_SHOW_TIME;
    m_junoAdviceBubble.setActive(true);
    m_junoHead.setActive(true);
}

void ORCar::hideAdvice() {
    m_junoAdviceBubble.setActive(false);
    m_junoHead.setActive(false);
}
