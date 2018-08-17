//
//  jnl_day_content_pane.cpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "jnl_day_content_pane.hpp"


DayContentPane::DayContentPane() {
    initList(&m_llContentItemList);
    addItem();
}

void DayContentPane::addItem() {
    DayContentItem* pNewDayContentItem = new DayContentItem();
    addChildWidget(pNewDayContentItem);
    addDataToList(&m_llContentItemList, pNewDayContentItem);
}

void DayContentPane::draw(uint8* buffer) {
    drawChildren(buffer);
}

void DayContentPane::update() {
    updateChildren();
}
