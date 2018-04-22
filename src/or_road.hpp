//
//  or_road.hpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_road_hpp
#define or_road_hpp

#include <stdio.h>
#include "iwidget.hpp"
#include "image.hpp"

#include "system.hpp"


class ORRoad : public IWidget {
public:
    ORRoad();
    
    void draw(uint8*);
    void update();
    
private:
    Image* m_roadImg;
    
    float m_fScrollingSpeed;
    float m_fXPos;
};


#endif /* or_road_hpp */
