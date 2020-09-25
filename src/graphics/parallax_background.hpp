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
#include "../core/idrawable.hpp"
#include "../graphics/sprite.hpp"
#include "../utils/linked_list.hpp"


typedef struct Layer {
    Image* img;
    int distance;
    vect2d_t v2dPos;
    vect2d_t offset;
} Layer;


class ParallaxBackground : public IWidget {
private:
    LinkedList m_lLayers;
    vect2d_t m_v2dCamPos;
	bool m_bDrawTransparency;
    
public:
    ParallaxBackground();
    ~ParallaxBackground();
    void addLayer(Image* layerImg, int distance, int offX = 0, int offY = 0);
    void deleteLayer(int layerNum);
    void update();
    void draw(drawbuffer* pBuffer);
    void setCamPos(int x, int y);
    vect2d_t getCamPos();
	void setTransparencyDrawing(bool newTransparency);
};

#endif /* parallax_background_hpp */
