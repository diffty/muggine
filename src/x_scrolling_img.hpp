//
//  x_scrolling_img.hpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef x_scrolling_img_hpp
#define x_scrolling_img_hpp

#include <stdio.h>
#include "iwidget.hpp"
#include "image.hpp"

#include "system.hpp"


class XScrollingImg : public IWidget {
private:
    Image* m_img;
    
    float m_fScrollSpeed;
    float m_fXPos;

public:
    XScrollingImg(Image* img, float fScrollSpeed);
    
    float getScrollSpeed();
    
    void draw(uint8*);
    void update();
   
};


#endif /* or_road_hpp */
