//
//  or_car.hpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_car_hpp
#define or_car_hpp

#include <stdio.h>

#include "iwidget.hpp"
#include "sprite.hpp"
#include "particle_system.hpp"
#include "text_bubble.hpp"

#define SHAKE_TIME 0.1
#define ADVICE_SHOW_TIME 5.0


class ORCar : public IWidget {
public:
    ORCar();
    ~ORCar();
    
    void update();
    void draw(uint8*);
    
    void showAdvice(char* szText);
    void hideAdvice();
    
private:
    Sprite m_carSpr;
    Sprite m_junoHead;
    TextBubble m_junoAdviceBubble;
    
    ParticleSystem m_particleSystem;
    
    int m_fShakeYOffset = 0.0;
    float m_fTimeBeforeNextShake = 0.0;
    float m_fTimeBeforeAdviceDismiss = 0.0;
};


#endif /* or_car_hpp */
