//
//  or_intro_animation.cpp
//  K-Mion
//
//  Created by DiFFtY on 24/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_intro_animation.hpp"
#include "system.hpp"
#include "rsc_manager.hpp"


ORIntroAnimation::ORIntroAnimation() {
    m_pTitleSpr = new Sprite(RscManager::get()->getSprShtRsc(4), 0, 100, ANIM_FROM_Y);
    m_iState = 0;

    m_fBeforeAnimTimer = 0.0;
    m_fAnimTitleUpTimer = 0.0;
    m_fAnimTitleStayTimer = 0.0;
    m_fAnimTitleFadeTimer = 0.0;

    addChildWidget(m_pTitleSpr);
}

void ORIntroAnimation::update() {
    if (m_bIsActive) {
        float* pCurrentTimer = NULL;
        
        switch (m_iState) {
            case 0:
                pCurrentTimer = &m_fBeforeAnimTimer;
                if (m_fBeforeAnimTimer > BEFORE_ANIM_TIMER) {
                     m_iState++;
                }
                else {
                    //float completion = (*pCurrentTimer / BEFORE_ANIM_TIMER);
                }
                break;
                
            case 1:
                pCurrentTimer = &m_fAnimTitleUpTimer;
                if (m_fAnimTitleUpTimer > ANIM_TITLE_UP_TIMER) {
                    m_iState++;
                }
                else {
                    float completion = (*pCurrentTimer / ANIM_TITLE_UP_TIMER);
                    m_pTitleSpr->translate(
                                           m_pTitleSpr->getRect()->getPos().x,
                                           ANIM_FROM_Y + (ANIM_TO_Y - ANIM_FROM_Y) * completion,
                                           TRANSFORM_ABS);
                }
                break;
                
            case 2:
                pCurrentTimer = &m_fAnimTitleStayTimer;
                if (m_fAnimTitleStayTimer > ANIM_TITLE_STAY_TIMER) {
                    m_iState++;
                }
                else {
                    //float completion = (*pCurrentTimer / ANIM_TITLE_STAY_TIMER);
                }
                break;
                
            case 3:
                pCurrentTimer = &m_fAnimTitleFadeTimer;
                if (m_fAnimTitleFadeTimer > ANIM_TITLE_FADE_TIMER) {
                    m_iState++;
                    m_pTitleSpr->setFrame(6);
                }
                else {
                    float completion = (*pCurrentTimer / ANIM_TITLE_FADE_TIMER);
                    m_pTitleSpr->setFrame((int) floor(completion * 6));
                }
                break;
                
            case 4:
                setActive(false);
        }
        
        if (pCurrentTimer != NULL) {
            *pCurrentTimer += System::get()->getDeltaTime();
        }
    }
    
    updateChildren();
}

void ORIntroAnimation::draw(uint8* buffer) {
    drawChildren(buffer);
}
