//
//  animatedsprite.cpp
//  Muggine
//
//  Created by DiFFtY on 08/10/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "animatedsprite.hpp"


AnimatedSprite::AnimatedSprite(SpriteSheet* pSprSht, vect2df_t vPos, float fPlaySpeed) :
    Sprite(pSprSht, 0, vPos)
{
    initList(&m_llAnimStates);
    m_fPlaySpeed = fPlaySpeed;
    
    if (m_llAnimStates.size > 0) {
        m_pCurrAnimState = (AnimationState*) m_llAnimStates.pHead->pData;
        m_uFrameNb = m_pCurrAnimState->uFrameStart;
    }
    else {
        m_uFrameNb = 0;
    }
}

AnimatedSprite::~AnimatedSprite() {
    
}

void AnimatedSprite::addState(char* szName, uint uFrameStart, uint uFrameEnd, uint uFPS, bool bLooped) {
    AnimationState newAnimState;
    
    newAnimState.szName = szName;
    newAnimState.bLooped = bLooped;
    newAnimState.uFrameStart = uFrameStart;
    newAnimState.uFrameEnd = uFrameEnd;
    newAnimState.uFPS = uFPS;
    
    addState(newAnimState);
}

void AnimatedSprite::addState(AnimationState animState) {
    addDataToList(&m_llAnimStates, new AnimationState(animState));
    
    if (m_llAnimStates.size == 1) {
        m_pCurrAnimState = (AnimationState*) m_llAnimStates.pHead->pData;
        m_uFrameNb = m_pCurrAnimState->uFrameStart;
    }
}

void AnimatedSprite::changeState(uint uStateId) {
    LLNode* pAnimStateNode = getNodeInList(&m_llAnimStates, uStateId);
    
    AnimationState* pAnimState = (AnimationState*) pAnimStateNode->pData;
    if (pAnimState != m_pCurrAnimState) {
        m_pCurrAnimState = pAnimState;
        m_uFrameNb = m_pCurrAnimState->uFrameStart;
    }
}

void AnimatedSprite::draw(uint8* fb) {
    Sprite::draw(fb);
}

void AnimatedSprite::update() {
    if (m_pCurrAnimState != NULL) {
        m_fCurrFrameTime += System::get()->getDeltaTime();
        
        if (m_fCurrFrameTime > (1. / m_pCurrAnimState->uFPS)) {
            m_uFrameNb++;
            
            if (m_uFrameNb > m_pCurrAnimState->uFrameEnd) {
                m_uFrameNb = m_pCurrAnimState->uFrameStart;
            }
            
            m_fCurrFrameTime = 0.;
        }
    }
    
    //printf("%i\n", m_uFrameNb);
}
