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
    m_carSpr(RscManager::get()->getImgRsc(7), 0, 0),
    m_junoHead(RscManager::get()->getImgRsc(16), 0, 0),
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
    m_fTimeBeforeAdviceDismiss = 0.0;
    m_fTimeBeforeNextShake = (float) SHAKE_TIME;
    
    getRect()->setSize(m_carSpr.getRect()->getSize().w, m_carSpr.getRect()->getSize().h);
    
    updateCollisionRect();
    
    m_fTimeBetweenSmokeParticles = TIME_BETWEEN_PARTICLES_NORMAL;
    m_fTimeBeforeNextSmokeParticle = m_fTimeBetweenSmokeParticles;
    
    m_eState = ECARSTATE_NORMAL;
}

ORCar::~ORCar() {
    
}

void ORCar::updateCollisionRect() {
    Rectf* vCarRect = getRect();

    m_collisionRect.setPos(vCarRect->getPos().x+7, vCarRect->getPos().y + 18);
    m_collisionRect.setSize(vCarRect->getSize().w-13, vCarRect->getSize().h - 17 - 10);
}

Rectf* ORCar::getCollisionRect() {
    return &m_collisionRect;
}

void ORCar::update() {
    updateChildren();
    
    m_fTimeBeforeNextShake -= System::get()->getDeltaTime();
    if (m_fTimeBeforeNextShake < 0.0) {
        translate(0.0, m_fShakeYOffset, TRANSFORM_REL);
        m_fTimeBeforeNextShake = (float) SHAKE_TIME;
        m_fShakeYOffset = - m_fShakeYOffset;
    }
    
    m_fTimeBeforeAdviceDismiss -= System::get()->getDeltaTime();
    if (m_fTimeBeforeAdviceDismiss < 0.0) {
        hideAdvice();
    }
    
    m_fTimeBeforeNextSmokeParticle -= System::get()->getDeltaTime();
    if (m_fTimeBeforeNextSmokeParticle < 0.0) {
        m_smokeParticleSystem
        .emitParticle(
                      RscManager::get()->getSprShtRsc(9),
                      getRect()->getPos().x,
                      getRect()->getPos().y + 15,
                      (float) System::get()->getRandInt(-20, -5),
                      (float) System::get()->getRandInt(-10, 10),
                      0,
                      2,
                      2
                      );

        switch (m_eState) {
            case ECARSTATE_NORMAL:
                m_fTimeBeforeNextSmokeParticle = TIME_BETWEEN_PARTICLES_NORMAL;
                break;
                
            case ECARSTATE_ACCELERATE:
                m_fTimeBeforeNextSmokeParticle = TIME_BETWEEN_PARTICLES_ACCELERATE;
                break;
                
            case ECARSTATE_BREAK:
                m_fTimeBeforeNextSmokeParticle = TIME_BETWEEN_PARTICLES_BREAK;
                break;
        }
    }
    
    m_smokeParticleSystem.update();
    m_trailParticleSystem.update();
}

void ORCar::draw(uint8* buffer) {
    m_smokeParticleSystem.draw(buffer);
    drawChildren(buffer);
    m_trailParticleSystem.draw(buffer);

    /*
    Color debugLineColor(255, 255, 255);
    
    quad2df_t collisionQuad = getCollisionRect()->getQuad2d();
    
    drawLine(buffer, collisionQuad.tl.x, collisionQuad.tl.y, collisionQuad.tr.x, collisionQuad.tr.y, &debugLineColor);
    drawLine(buffer, collisionQuad.tr.x, collisionQuad.tr.y, collisionQuad.br.x, collisionQuad.br.y, &debugLineColor);
    drawLine(buffer, collisionQuad.br.x, collisionQuad.br.y, collisionQuad.bl.x, collisionQuad.bl.y, &debugLineColor);
    drawLine(buffer, collisionQuad.bl.x, collisionQuad.bl.y, collisionQuad.tl.x, collisionQuad.tl.y, &debugLineColor);
    */
}

void ORCar::onNormal() {
    m_eState = ECARSTATE_NORMAL;
}

void ORCar::onAccelerating() {
    /*m_trailParticleSystem
    .emitParticle(
                  RscManager::get()->getSprShtRsc(34),
                  getRect()->getPos().x + 2,
                  getRect()->getPos().y + 21,
                  -1,
                  0,
                  0,
                  0,
                  1
                  );*/
    
    m_eState = ECARSTATE_ACCELERATE;
}

void ORCar::onBreaking() {
    m_eState = ECARSTATE_BREAK;
}

void ORCar::translate(float x, float y, ETransformMode transformMode) {
    IWidget::translate(x, y, transformMode);
    
    updateCollisionRect();
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

ECarState ORCar::getState() {
    return m_eState;
}
