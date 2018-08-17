//
//  jnl_calendar_view.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef jnl_calendar_view_hpp
#define jnl_calendar_view_hpp

#include <stdio.h>
#include "common_types.hpp"
#include "iwidget.hpp"
#include "text.hpp"
#include "drawing.hpp"
#include "jnl_calendar_day.hpp"
#include "rsc_manager.hpp"
#include "datetime_tools.hpp"


class CalendarView : public IWidget {
public:
    CalendarView(float fPosX, float fPosY, float fSizeW, float fSizeH);
    
    void draw(uint8* buffer);
    void update();
};


#endif /* jnl_calendar_view_hpp */
