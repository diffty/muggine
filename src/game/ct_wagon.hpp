//
//  ct_wagon.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 24/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef ct_wagon_hpp
#define ct_wagon_hpp

#include <stdio.h>
#include "ct_character.hpp"
#include "../core/iwidget.hpp"
#include "../graphics/drawbuffer.hpp"


class CTWagon : public IWidget {
public:
    CTWagon();
    ~CTWagon();
    
    void addCharacter(int iSlotPosX, int iSlotPosY);
    int getSlotId(int iSlotPosX, int iSlotPosY);
    vect2d_t getSlotPosition(int iSlotPosX, int iSlotPosY);
    vect2d_t getSlotPosition(int iSlotId);
    
    void draw(drawbuffer* pBuffer);

private:
    DrawBuffer drawBuffer;
    Sprite* bufferSpr;
    CTCharacter** m_aSlots;
    
    bool m_bIsDoorOpened = false;
    
    int m_iNbSlotsX = 4;
    int m_iNbSlotsY = 8;
};

#endif /* ct_wagon_hpp */
