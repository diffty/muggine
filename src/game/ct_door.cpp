//
//  ct_door.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 27/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "ct_door.hpp"
#include "../manager/rsc_manager.hpp"


CTDoor::CTDoor(int x, int y)
    : AnimatedSprite(RscManager::get()->getSprShtRsc("door"), x, y)
{
    m_classInfo.setClassTypeName("CTDoor");
    addState("idle_closed", 0, 0, 1, false, NULL, NULL);
    addState("opening", 0, 6, 10, false, NULL, NULL);
    addState("opened", 6, 6, 1, false, NULL, NULL);
    pause();
}

bool CTDoor::isOpened() {
    return strcmp(getState()->szName, "idle_closed") != 0;
}
