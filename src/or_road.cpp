//
//  or_road.cpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_road.hpp"

#include "rsc_manager.hpp"


ORRoad::ORRoad() {
    m_roadImg = RscManager::get()->getImgRsc(6);
    m_fScrollSpeed = 100.0;
}

float ORRoad::getScrollSpeed() {
    return m_fScrollSpeed;
}

void ORRoad::update() {
    size2df_t roadImgSize = m_roadImg->getSize();

    if (m_fXPos > 320)
        m_fXPos -= roadImgSize.w;
    
    if (m_fXPos <= 0)
        m_fXPos += roadImgSize.w;
    
    m_fXPos -= System::get()->getDeltaTime() * m_fScrollSpeed;
}

void ORRoad::draw(uint8* fb) {
    size2df_t roadImgSize = m_roadImg->getSize();
    
    int fStartPosX = m_fXPos - roadImgSize.w;
    int fCurXPos = fStartPosX;
    
    while (fCurXPos < 320) {
        m_roadImg->draw(fb, (int) fCurXPos, 150, false, true);
        fCurXPos += roadImgSize.w;
    }
}
