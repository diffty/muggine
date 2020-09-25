//
//  x_scrolling_img.cpp
//  K-Mion
//
//  Created by DiFFtY on 21/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "x_scrolling_img.hpp"

#include "../manager/rsc_manager.hpp"


XScrollingImg::XScrollingImg(Image* img, float fScrollSpeed) {
    m_img = img;
    m_fScrollSpeed = fScrollSpeed;
}

float XScrollingImg::getScrollSpeed() {
    return m_fScrollSpeed;
}

void XScrollingImg::update() {
    size2df_t imgSize = m_img->getSize();

    if (m_fXPos > SCREEN_WIDTH)
        m_fXPos -= imgSize.w;
    
    if (m_fXPos <= 0)
        m_fXPos += imgSize.w;
    
    m_fXPos -= System::get()->getDeltaTime() * m_fScrollSpeed;
}

void XScrollingImg::draw(drawbuffer* pBuffer) {
    size2df_t imgSize = m_img->getSize();
    
    int fStartPosX = m_fXPos - imgSize.w;
    int fCurXPos = fStartPosX;
    
    while (fCurXPos < SCREEN_WIDTH) {
        m_img->draw(pBuffer, (int) fCurXPos, getRect()->getPos().y, false, true);
        fCurXPos += imgSize.w;
    }
}
