//
//  jnl_day_content_pane.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef jnl_day_content_pane_hpp
#define jnl_day_content_pane_hpp

#include <stdio.h>
#include "iwidget.hpp"
#include "jnl_day_content_item.hpp"


class DayContentPane : public IWidget {
    LinkedList m_llContentItemList;
    
public:
    DayContentPane();
    
    void addItem();
    
    void draw(uint8* buffer);
    void update();
};


#endif /* jnl_day_content_pane_hpp */
