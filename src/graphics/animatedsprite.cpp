//
//  animatedsprite.cpp
//  Muggine
//
//  Created by DiFFtY on 08/10/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "animatedsprite.hpp"


AnimatedSprite::AnimatedSprite(SpriteSheet* pSprSht, vect2df_t vPos, float fPlaySpeed, char* szName) :
    Sprite(pSprSht, 0, vPos.x, vPos.y, szName)
{
	init(pSprSht, 0, vPos.x, vPos.y);
}

AnimatedSprite::AnimatedSprite(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed, char* szName) :
	Sprite(pSprSht, 0, fXPos, fYPos, szName)
{
    init(pSprSht, 0, fXPos, fYPos);
}

AnimatedSprite::~AnimatedSprite() {
	destroyAllStates();
}

void AnimatedSprite::init(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed) {
    initList(&m_llAnimStates);
    m_fPlaySpeed = fPlaySpeed;
    
    m_fCurrFrameTime = 0.;
    
    if (m_llAnimStates.size > 0) {
        m_pCurrAnimState = (AnimationState*) m_llAnimStates.pHead->pData;
        setFrame(m_pCurrAnimState->uFrameStart);
    }
    else {
        setFrame(0);
    }
}

void AnimatedSprite::addState(const char* szName, uint uFrameStart, uint uFrameEnd, uint uFPS, bool bLooped, void (*pOnAnimEndCallback)(void*), void* pOnAnimEndCallbackArg) {
    AnimationState newAnimState;
    
    newAnimState.szName = szName;
    newAnimState.bLooped = bLooped;
    newAnimState.uFrameStart = uFrameStart;
    newAnimState.uFrameEnd = uFrameEnd;
    newAnimState.uFPS = uFPS;
    newAnimState.pOnAnimEndCallback = pOnAnimEndCallback;
    newAnimState.pOnAnimEndCallbackArg = pOnAnimEndCallbackArg;
    
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

AnimationState* AnimatedSprite::getState() {
    return m_pCurrAnimState;
}

void AnimatedSprite::draw(drawbuffer* pBuffer) {
    Sprite::draw(pBuffer);
}

void AnimatedSprite::update() {
	updateChildren();

    if (m_pCurrAnimState != NULL) {
        m_fCurrFrameTime += System::get()->getDeltaTime();
        
		int iCurrFrame = getFrame();
		
		if (m_fCurrFrameTime > (1. / m_pCurrAnimState->uFPS)) {
			iCurrFrame++;
            
            if (iCurrFrame > m_pCurrAnimState->uFrameEnd) {
                if (m_pCurrAnimState->bLooped) {
                    iCurrFrame = m_pCurrAnimState->uFrameStart;
                }
                else {
                    iCurrFrame = m_pCurrAnimState->uFrameEnd;
                }
                
                if (m_pCurrAnimState->pOnAnimEndCallback != NULL) {
                    (*m_pCurrAnimState->pOnAnimEndCallback)(m_pCurrAnimState->pOnAnimEndCallbackArg);
                }
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
