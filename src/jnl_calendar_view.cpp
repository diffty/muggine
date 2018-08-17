//
//  jnl_calendar_view.cpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "jnl_calendar_view.hpp"


CalendarView::CalendarView(float fPosX, float fPosY, float fSizeW, float fSizeH) :
    IWidget(fPosX, fPosX, fSizeW, fSizeH)
{
    RscManager* pRscManager = RscManager::get();
    
    int iFirstDayOfMonth = (dayOfWeek(2018, 06, 01) + 6) % 7;
    
    for (int i = 0; i < 31; i++) {
        CalendarDay* calendarDay = new CalendarDay(i+1, pRscManager->getFontRsc(0));
        
        calendarDay->translate(
                               fPosX + (fSizeW / 7.) * ((iFirstDayOfMonth + i) % 7),
                               fPosY + (fSizeH / 4.) * ((iFirstDayOfMonth + i) / 7),
                               TRANSFORM_ABS
                               );
        
        addChildWidget(calendarDay);
    }
}


void CalendarView::draw(uint8* buffer) {
    vect2df_t vPos = m_rect.getPos();
    size2df_t sSize = m_rect.getSize();
    
    Color color(255, 255, 255);
    
    // drawLine(buffer, vPos.x + sSize.w, vPos.y, sSize.w, sSize.h, &color);
    
    drawChildren(buffer);
}

void CalendarView::update() {
    updateChildren();
}
