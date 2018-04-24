//
//  or_pickup_items_manager.hpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_pickup_items_manager_hpp
#define or_pickup_items_manager_hpp

#include <stdio.h>

#include "iwidget.hpp"
#include "or_pickup_item.hpp"
#include "linked_list.hpp"


#define MIN_SPAWN_TIME 0.15
#define MAX_SPAWN_TIME 0.9
#define TOMATO_POPULATION_AVAILABILITY 100


class ORPickupItemsManager : public IWidget {
public:
    ORPickupItemsManager();
    ~ORPickupItemsManager();
    
    void spawnItem();
    
    LinkedList* getItemsList();
    void onItemPicked(ORPickupItem* pPickedUpItem);

    void update();
    void draw(uint8* buffer);

private:
    LinkedList m_llPickupList;
    
    float m_fTimeBeforeNextItemSpawn;
};


#endif /* or_pickup_items_manager_hpp */
