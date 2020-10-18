//
//  gm_gem.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 06/10/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef gm_gem_hpp
#define gm_gem_hpp

#include <stdio.h>

#include "../core/common_types.hpp"
#include "../core/iwidget.hpp"
#include "../graphics/spritesheet.hpp"
#include "../graphics/sprite.hpp"


class Gem : public Sprite {
public:
    Gem(SpriteSheet* pGemSprSht, int iColorId = -1);
    ~Gem();
    
    void moveTo(vect2d_t vDestPos);
    void moveToGridRelative(vect2d_t vRelPos);
    void update();
    bool receiveTouchInput(vect2d_t touchPt);

    int getColorId();

    void onPress();
    void onRelease();
    

private:
    int m_iColorId = 0;
    SpriteSheet* m_pGemSprSht = NULL;
    bool m_bPressedThisLoop = false;
    bool m_bIsHovered = false;
    bool m_bIsPressed = false;
    
    vect2d_t m_vMoveTo;
    bool m_bIsMoving = false;
};

#endif /* gm_gem_hpp */
