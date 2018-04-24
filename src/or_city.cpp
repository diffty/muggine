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
    
    int minStateId = 0, maxStateId = 0;
    
    switch (eNewBuildingType) {
        case EBuildingType_House:
            buildingRscId = 22 + System::get()->getRandInt(0, 2);
            minStateId = 0;
            maxStateId = 2;
            break;
            
        case EBuildingType_Building:
            buildingRscId = 22 + System::get()->getRandInt(0, 2);
            minStateId = 0;
            maxStateId = 2;
            break;
            
        case EBuildingType_Tree:
            buildingRscId = 19 + System::get()->getRandInt(0, 2);
            minStateId = 0;
            maxStateId = 2;
            break;
            
        case EBuildingType_Factory:
            if (System::get()->getRandInt(0, 2) == 0)
                buildingRscId = 14;
            else
                buildingRscId = 21;
            
            minStateId = 0;
            maxStateId = 0;
            break;
    }
    
    SpriteSheet* pBuildingSprSht = RscManager::get()->getSprShtRsc(buildingRscId);
    
    // TODO: Faire le Y random en fonction de la taille du batiment
    vect2df_t vNewBuildingPos;
    vNewBuildingPos.x = 330;
    vNewBuildingPos.y = 160 - pBuildingSprSht->getSize().h + System::get()->getRandInt(0, 30);
    
    //int iFrameId = (int) ((1.0 - getCityHealth()) * (((float) maxStateId + 1) - 0.01));
    //WARNING: ce truc pourrait péter à la gueule à un moment
    int iFrameId = (int) floor((1.0 - getCityHealth()) * (maxStateId + 0.99));
    printf("%f, %i\n", getCityHealth(), iFrameId);
    Sprite* pNewBuildingSpr = new Sprite(pBuildingSprSht, iFrameId, vNewBuildingPos);
    addChildWidget(pNewBuildingSpr);
    addDataToList(&m_llBuildingList, pNewBuildingSpr);
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
    
    if (m_fCitySize < ORGameMode::get()->getPopulationValue()) {
        m_fCitySize = ORGameMode::get()->getPopulationValue();
        m_fTimeBeforeCitySizeDecrease = TIME_BEFORE_CITY_SIZE_DECREASE;
    }
    else {
        if (m_fTimeBeforeCitySizeDecrease < 0.0) {
            m_fCitySize = maxf(0.0, m_fCitySize - System::get()->getDeltaTime() * 2.0);
        }
        else {
            m_fTimeBeforeCitySizeDecrease -= System::get()->getDeltaTime();
        }
    }
    
    getCityHealth();
    
    updateChildren();
}

float ORCity::getCityHealth() {
    if (m_fCitySize == 0) {
        return 1.;
    }
    else {
        return ORGameMode::get()->getPopulationValue() / (m_fCitySize);
    }
}

float ORCity::getCitySize() {
    return m_fCitySize;
}

void ORCity::draw(uint8* buffer) {
    drawChildren(buffer);
}
