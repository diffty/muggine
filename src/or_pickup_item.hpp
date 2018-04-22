//
//  or_pickup_item.hpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_pickup_item_hpp
#define or_pickup_item_hpp

#include <stdio.h>

#include "sprite.hpp"


class ORPickupItem : public Sprite {
public:
    ORPickupItem(Image* pItemImg);
    
    void onPickup();
};

#endif /* or_pickup_item_hpp */
