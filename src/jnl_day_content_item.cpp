//
//  jnl_day_content_item.cpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "jnl_day_content_item.hpp"


DayContentItem::DayContentItem() {
    addChildWidget(&m_editableTextWidget);
}

void DayContentItem::draw(uint8* buffer) {
    drawChildren(buffer);
}

void DayContentItem::update() {
    updateChildren();
}
