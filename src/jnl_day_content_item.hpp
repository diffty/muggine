//
//  jnl_day_content_item.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef jnl_day_content_item_hpp
#define jnl_day_content_item_hpp

#include <stdio.h>
#include "iwidget.hpp"
#include "editable_text.hpp"


class DayContentItem : public IWidget {
private:
    EditableText m_editableTextWidget;
    
public:
    DayContentItem();
    
    void draw(uint8* buffer);
    void update();

};


#endif /* jnl_day_content_item_hpp */
