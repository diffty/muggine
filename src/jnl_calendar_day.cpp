//
//  jnl_calendar_day.cpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "jnl_calendar_day.hpp"


CalendarDay::CalendarDay(int iDayNum, Font* pFont) {
    m_pDayNumberText = new Text(iDayNum, pFont, 0, 0);

    addChildWidget(m_pDayNumberText);
    
}

void CalendarDay::draw(uint8* buffer) {
    drawChildren(buffer);
}

void CalendarDay::update() {
    updateChildren();
}
