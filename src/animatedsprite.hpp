//
//  animatedsprite.hpp
//  Muggine
//
//  Created by DiFFtY on 08/10/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <stdio.h>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "linked_list.hpp"
#include "system.hpp"


struct AnimationState {
    const char* szName;
    uint uFrameStart;
    uint uFrameEnd;
    uint uFPS;
    bool bLooped;

    void (*pOnAnimEndCallback)(void*);
    void *pOnAnimEndCallbackArg;
};


class AnimatedSprite : public Sprite {
private:
    LinkedList m_llAnimStates;
    AnimationState* m_pCurrAnimState;

    float m_fPlaySpeed;
    float m_fCurrFrameTime;
    
public:
    AnimatedSprite(SpriteSheet* pSprSht, vect2df_t vPos, float fPlaySpeed = 1.);
	AnimatedSprite(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed = 1.);
    ~AnimatedSprite();
    
    void init(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed = 1.);
    
    void addState(const char* szName, uint uFrameStart, uint uFrameEnd, uint uFPS = 1., bool bLooped = true, void (*pOnAnimEndCallback)(void*) = NULL, void* pOnAnimEndCallbackArg = NULL);
    void addState(AnimationState animState);
    void changeState(uint uStateId);
    AnimationState* getState();
    
    void update();
    void draw(uint8* buffer);

	void destroyAllStates();
};


#endif /* animatedsprite_hpp */
