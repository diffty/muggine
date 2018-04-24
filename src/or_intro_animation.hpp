//
//  or_intro_animation.hpp
//  K-Mion
//
//  Created by DiFFtY on 24/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_intro_animation_hpp
#define or_intro_animation_hpp

#include <stdio.h>
#include "iwidget.hpp"
#include "image.hpp"
#include "sprite.hpp"


#define BEFORE_ANIM_TIMER 2.0 // 3.0
#define ANIM_TITLE_UP_TIMER 3.0
#define ANIM_TITLE_STAY_TIMER 10.0
#define ANIM_TITLE_FADE_TIMER 0.75
#define ANIM_FROM_Y 200
#define ANIM_TO_Y 30


class ORIntroAnimation : public IWidget {
public:
    ORIntroAnimation();
    
    void update();
    void draw(uint8* buffer);
    
private:
    Sprite* m_pTitleSpr;
    int m_iState = 0;
    float m_fBeforeAnimTimer = 0.;
    float m_fAnimTitleUpTimer = 0.;
    float m_fAnimTitleStayTimer = 0.;
    float m_fAnimTitleFadeTimer = 0.;
};


#endif /* or_intro_animation_hpp */
