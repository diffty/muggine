//
//  parallax-background.hpp
//  Muggine
//
//  Created by DiFFtY on 03/09/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#ifndef parallax_background_hpp
#define parallax_background_hpp

#include <stdio.h>
#include "idrawable.hpp"
#include "sprite.hpp"
#include "linked_list.hpp"


typedef struct Layer {
    Image* img;
    int distance;
    vect2d_t v2dPos;
} Layer;


class ParallaxBackground : IWidget {
private:
    LinkedList m_lLayers;
    vect2d_t m_v2dCamPos;
    
public:
    ParallaxBackground();
    ~ParallaxBackground();
    void addLayer(Image* layerImg, int distance);
    void deleteLayer(int layerNum);
    void update();
    void draw(uint8* buffer);
    
};

#endif /* parallax_background_hpp */
