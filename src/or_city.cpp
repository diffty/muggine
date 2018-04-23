//
//  or_city.cpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_city.hpp"


ORCity::ORCity() {
    initList(&m_llBuildingList);
    
    m_fCityHealth = CITY_START_HEALTH;
    m_fSpawnAmount = SPAWN_START_AMOUNT;
    m_fCitySize = 0.0;
    m_iCityLevel = 1;
}

void ORCity::generateNewBuilding(EBuildingType eNewBuildingType) {
    int buildingRscId = -1;
    
    switch (eNewBuildingType) {
        case EBuildingType_House:
            buildingRscId = 15;
            break;
            
        case EBuildingType_Building:
            buildingRscId = 15;
            break;
            
        case EBuildingType_Tree:
            buildingRscId = 13;
            break;
            
        case EBuildingType_Factory:
            buildingRscId = 14;
            break;
    }
    
    Image* pBuildingImg = RscManager::get()->getImgRsc(buildingRscId);
    
    vect2df_t vNewBuildingPos;
    vNewBuildingPos.x = 330;
    vNewBuildingPos.y = 160 - pBuildingImg->getSize().h + System::get()->getRandInt(0, 20);
    
    Sprite* pNewBuildingSpr = new Sprite(pBuildingImg, vNewBuildingPos);
    addChildWidget(pNewBuildingSpr);
    addDataToList(&m_llBuildingList, pNewBuildingSpr);
    
    m_fCitySize += 1.0;
}

void ORCity::update() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    LLNode* pCurrNode = m_llBuildingList.pHead;
    LLNode* pPrevNode;
    
    while (pCurrNode != NULL) {
        Sprite* pCurrSpr = (Sprite*) pCurrNode->pData;
        pCurrSpr->translate(fDeltaTime * -15, 0, TRANSFORM_REL);
        
        pPrevNode = pCurrNode;
        pCurrNode = pCurrNode->pNext;

        if (pCurrSpr->getRect()->getPos().x < -100) {
            removeChildWidget(pCurrSpr);
            removeNodeFromList(&m_llBuildingList, pPrevNode);
            delete pPrevNode;
            delete pCurrSpr;
        }
    }
    
    getCityHealth();
    
    updateChildren();
}

float ORCity::getCityHealth() {
    return ORGameMode::get()->getPopulationValue() / (m_fCitySize * 0.5);
}

float ORCity::getCitySize() {
    return m_fCitySize;
}

float ORCity::getPopulationHappiness() {
    ORGameMode* pGameMode = ORGameMode::get();
    
    int iHouse = pGameMode->getHouseValue();
    int iEcology = pGameMode->getEcologyValue();
    int iIndustry = pGameMode->getIndustryValue();
    
    
    
    return 0;
}

void ORCity::draw(uint8* buffer) {
    drawChildren(buffer);
}
