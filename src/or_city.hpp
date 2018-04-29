//
//  or_city.hpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef or_city_hpp
#define or_city_hpp

#define CITY_START_HEALTH 100.0
#define SPAWN_START_AMOUNT 0.0
#define CITY_SIZE_DECREASE_COEF 0.001
#define TIME_BEFORE_CITY_SIZE_DECREASE 3


#include <stdio.h>

#include "linked_list.hpp"
#include "iwidget.hpp"
#include "spritesheet.hpp"


class ORGameMode;


enum EBuildingType {
    EBuildingType_House,
    EBuildingType_Building,
    EBuildingType_Tree,
    EBuildingType_Factory,
};


class ORCity : public IWidget {    
private:
    LinkedList m_llBuildingList;
    
    float m_fCityHealth;
    float m_fCitySize;
    int m_iCityLevel;
    float m_fSpawnAmount;
    float m_fTimeBeforeCitySizeDecrease;

public:
    ORCity();
    
    void update();
    void draw(uint8* buffer);

    void generateNewBuilding(EBuildingType eNewBuildingType);
    
    float getCityHealth();
    float getCitySize();
};


#include "or_game_mode.hpp"

#endif /* or_city_hpp */
