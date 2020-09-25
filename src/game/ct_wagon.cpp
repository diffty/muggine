//
//  ct_wagon.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 24/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "ct_wagon.hpp"

#include "../manager/rsc_manager.hpp"


CTWagon::CTWagon() {
    drawBuffer.createBuffer(200, 200);
    drawBuffer.clear();
    
    bufferSpr = new Sprite(&drawBuffer, 0, 0);
    addChildWidget(bufferSpr);
    
    int iNbSlots = m_iNbSlotsX * m_iNbSlotsY;
    m_aSlots = new CTCharacter*[iNbSlots];
    for (int i = 0; i < iNbSlots; i++) {
        m_aSlots[i] = NULL;
    }
    
    RscManager* rscManager = RscManager::get();
    Image* wagonImg = rscManager->getImgRsc("wagon");
    Image* seatImg = rscManager->getImgRsc("seats-wagon");

    wagonImg->draw(drawBuffer.getBuffer(), 0, 0, false, true);

    for (int y = 0; y < m_iNbSlotsY; y++) {
        for (int x = 0; x < m_iNbSlotsX / 2; x++) {
            vect2d_t pos = getSlotPosition(x*2, y);
            seatImg->draw(drawBuffer.getBuffer(), pos.x + 4, pos.y + 12, false, true);
        }
    }
    
    for (int y = 0; y < m_iNbSlotsY; y++) {
        for (int x = 0; x < m_iNbSlotsX; x++) {
            addCharacter(x, y);
        }
    }
}

CTWagon::~CTWagon() {
    for (int i = 0; i < m_iNbSlotsX * m_iNbSlotsY; i++) {
        if (m_aSlots[i] != NULL) {
            delete (CTCharacter*) m_aSlots[i];
        }
    }
    delete m_aSlots;
}

void CTWagon::addCharacter(int iSlotPosX, int iSlotPosY) {
    vect2d_t vSlotPos = getSlotPosition(iSlotPosX, iSlotPosY);
    CTCharacter* pNewChar = new CTCharacter();
    int iSlotId = getSlotId(iSlotPosX, iSlotPosY);
    m_aSlots[iSlotId] = pNewChar;
    
    //addChildWidget(pNewChar);
    
    size2d_t bufferSize = drawBuffer.getSizei();

    pNewChar->translate(vSlotPos.x, vSlotPos.y);
    pNewChar->draw(drawBuffer.getBuffer());
}

vect2d_t CTWagon::getSlotPosition(int iSlotPosX, int iSlotPosY) {
    return getSlotPosition(iSlotPosY * m_iNbSlotsX + iSlotPosX);
}

int CTWagon::getSlotId(int iSlotPosX, int iSlotPosY) {
    return iSlotPosY * m_iNbSlotsX + iSlotPosX;
}

vect2d_t CTWagon::getSlotPosition(int iSlotId) {
    int x = iSlotId % 4;
    int y = iSlotId / 4;
    
    vect2d_t vSlotPosition;
    
    if (x >= 2) {
        vSlotPosition.x = 54 + (x % 2) * 16;
    }
    else {
        vSlotPosition.x = x * 16;
    }
    vSlotPosition.y = y * 16;
    
    vSlotPosition.x += 7;
    vSlotPosition.y += 10;
    
    return vSlotPosition;
}

void CTWagon::draw(drawbuffer *pBuffer) {
    if (m_bIsActive) {
        drawChildren(pBuffer);
    }
}
