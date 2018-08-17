//
//  jnl_calendar_day.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef jnl_calendar_day_hpp
#define jnl_calendar_day_hpp

#include <stdio.h>

#include "common_types.hpp"
#include "iwidget.hpp"
#include "text.hpp"
#include "font.hpp"



class CalendarDay : public IWidget {
public:
    CalendarDay(int iDayNum, Font* pFont);
    
    void draw(uint8* buffer);
    void update();
    
private:
    Text* m_pDayNumberText;
};


#endif /* jnl_calendar_day_hpp */
