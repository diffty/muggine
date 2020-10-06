//
//  ct_door.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 27/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef ct_door_hpp
#define ct_door_hpp

#include <stdio.h>

#include "../graphics/animatedsprite.hpp"

class CTDoor : public AnimatedSprite {
public:
    CTDoor(int x, int y);
    bool isOpened();
};

#endif /* ct_door_hpp */
