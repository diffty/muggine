//
//  gm_gem.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 06/10/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "gm_gem.hpp"
#include "../core/input.hpp"
#include "../core/system.hpp"

#include "../gems/gm_gemboard.hpp"


Gem::Gem(SpriteSheet* pGemSprSht, int iColorId) : Sprite(pGemSprSht, 0, 0, 0) {
    m_pGemSprSht = pGemSprSht;
    
    if (iColorId >= 0)
        m_iColorId = iColorId;
    else
        m_iColorId = System::get()->getRandInt(0, (int) pGemSprSht->getLength());
    
    setFrame(m_iColorId);
}

Gem::~Gem() {
    
}

void Gem::moveToGridRelative(vect2d_t vRelPos) {
    
}

void Gem::moveTo(vect2d_t vDestPos) {
    m_vMoveTo = vDestPos;
    m_bIsMoving = true;
}

void Gem::update() {
    if (m_bIsMoving) {
        float fDeltaTime = System::get()->getDeltaTime();
        vect2df_t vCurrPos = m_rect.getPos();
        
        float fDist = sqrt((m_vMoveTo.x - vCurrPos.x) * (m_vMoveTo.x - vCurrPos.x) + (m_vMoveTo.y - vCurrPos.y) * (m_vMoveTo.y - vCurrPos.y));
        
        if (fDist < 0.5) {
            translate(m_vMoveTo.x, m_vMoveTo.y, TRANSFORM_ABS);
            m_bIsMoving = false;
        }
        else {
            /*translate(((m_vMoveTo.x - vCurrPos.x) * 5.0) * fDeltaTime,
                      ((m_vMoveTo.y - vCurrPos.y) * 5.0) * fDeltaTime,
                      TRANSFORM_REL);*/
            vect2df_t velVect = getNormalizedVect({(m_vMoveTo.x - vCurrPos.x), (m_vMoveTo.y - vCurrPos.y)});
            translate((velVect.x * 10.0) * fDeltaTime,
                      (velVect.y * 10.0) * fDeltaTime,
                      TRANSFORM_REL);
        }
    }
    
    updateChildren();

    if (m_bIsPressed) {
        Input* pInputSys = System::get()->getInputSys();

        if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
            onRelease();
            m_bIsPressed = false;
        }
    }

    if (m_bIsHovered) {
        Input* pInputSys = System::get()->getInputSys();
        vect2d_t vMousePos = pInputSys->getCurrInputPos();

        if (!m_rect.isPointInRect(vMousePos.x, vMousePos.y)) {
            m_bIsHovered = false;
        }
    }

    m_bPressedThisLoop = false;
}

bool Gem::receiveTouchInput(vect2d_t touchPt) {
    if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
        if (!m_bIsPressed) onPress();
        m_bIsPressed = true;
        m_bPressedThisLoop = true;

        return true;
    }
    else {
        return false;
    }
}

int Gem::getColorId() {
    return m_iColorId;
}

void Gem::onPress() {
    GemBoard* parentBoard = (GemBoard*) getParentWidget();
    parentBoard->destroyGem(this);
    printf("%p\n", this);
}

void Gem::onRelease() {
    
}
