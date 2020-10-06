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
#include "../graphics/sprite.hpp"
#include "../graphics/spritesheet.hpp"
#include "../utils/linked_list.hpp"
#include "../core/system.hpp"


struct AnimationState {
    const char* szName;
    uint uFrameStart;
    uint uFrameEnd;
    uint uFPS;
    bool bLooped;
    int id;

    void (*pOnAnimEndCallback)(void*);
    void *pOnAnimEndCallbackArg;
};


class AnimatedSprite : public Sprite {
private:
    LinkedList m_llAnimStates;
    AnimationState* m_pCurrAnimState;
    
    bool m_bIsPlaying = false;
    float m_fPlaySpeed;
    float m_fCurrFrameTime;
    
public:
    AnimatedSprite(SpriteSheet* pSprSht, vect2df_t vPos, float fPlaySpeed = 1., char* szName = NULL);
	AnimatedSprite(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed = 1., char* szName = NULL);
    ~AnimatedSprite();
    
    void init(SpriteSheet* pSprSht, float fXPos, float fYPos, float fPlaySpeed = 1.);
    
    void addState(const char* szName, uint uFrameStart, uint uFrameEnd, uint uFPS = 1., bool bLooped = true, void (*pOnAnimEndCallback)(void*) = NULL, void* pOnAnimEndCallbackArg = NULL);
    void addState(AnimationState animState);
    void changeState(uint uStateId);
    AnimationState* getState();
    int getStateId();
    
    void play();
    void pause();
    
    void update();
    void draw(drawbuffer* pBuffer);

	void destroyAllStates();
};


#endif /* animatedsprite_hpp */
