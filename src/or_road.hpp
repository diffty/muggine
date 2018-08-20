//
//  or_road.hpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_road_hpp
#define or_road_hpp

#define SCROLL_SPEED_NORMAL         100.0
#define SCROLL_SPEED_ACCELERATION   130.0
#define SCROLL_SPEED_BREAK          70.0

#include <stdio.h>
#include "iwidget.hpp"
#include "image.hpp"

#include "system.hpp"


class ORRoad : public IWidget {	    
private:
    Image* m_roadImg;
    
    float m_fScrollSpeed;
    float m_fXPos;

public:
    ORRoad();
    
    float getScrollSpeed();
    void setScrollSpeed(float fScrollSpeed);
    
    void draw(uint8*);
    void update();
};


#endif /* or_road_hpp */
