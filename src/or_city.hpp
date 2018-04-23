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
public:
    ORCity();
    
    void update();
    void draw(uint8* buffer);

    void generateNewBuilding(EBuildingType eNewBuildingType);
    
    float getCityHealth();
    float getCitySize();
    
private:
    ORGameMode* m_pGameMode;
    
    LinkedList m_llBuildingList;
    
    float m_fCityHealth = CITY_START_HEALTH;
    float m_fCitySize = 0;
    int m_iCityLevel = 1;
    float m_fSpawnAmount = SPAWN_START_AMOUNT;
};


#include "or_game_mode.hpp"

#endif /* or_city_hpp */
