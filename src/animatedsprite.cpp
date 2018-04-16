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
		setFrame(m_pCurrAnimState->uFrameStart);
    }
    else {
		setFrame(0);
    }
}

AnimatedSprite::~AnimatedSprite() {
	destroyAllStates();
}

void AnimatedSprite::addState(const char* szName, uint uFrameStart, uint uFrameEnd, uint uFPS, bool bLooped) {
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
		setFrame(m_pCurrAnimState->uFrameStart);
    }
}

void AnimatedSprite::changeState(uint uStateId) {
    LLNode* pAnimStateNode = getNodeInList(&m_llAnimStates, uStateId);
    
    AnimationState* pAnimState = (AnimationState*) pAnimStateNode->pData;
    if (pAnimState != m_pCurrAnimState) {
        m_pCurrAnimState = pAnimState;
		setFrame(m_pCurrAnimState->uFrameStart);
    }
}

void AnimatedSprite::draw(uint8* buffer) {
    Sprite::draw(buffer);
}

void AnimatedSprite::update() {
	updateChildren();

    if (m_pCurrAnimState != NULL) {
        m_fCurrFrameTime += System::get()->getDeltaTime();
        
		int iCurrFrame = getFrame();
		
		if (m_fCurrFrameTime > (1. / m_pCurrAnimState->uFPS)) {
			iCurrFrame++;
            
            if (iCurrFrame > m_pCurrAnimState->uFrameEnd) {
				iCurrFrame = m_pCurrAnimState->uFrameStart;
            }

			setFrame(iCurrFrame);
            
            m_fCurrFrameTime = 0.;
        }
    }
}

void AnimatedSprite::destroyAllStates() {
	LLNode* pCurrNode = m_llAnimStates.pHead;

	while (pCurrNode != NULL) {
		delete ((AnimationState*) pCurrNode->pData);
		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llAnimStates);
}
