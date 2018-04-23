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
            buildingRscId = 14;
            minStateId = 0;
            maxStateId = 0;
            break;
    }
    
    SpriteSheet* pBuildingSprSht = RscManager::get()->getSprShtRsc(buildingRscId);
    
    // TODO: Faire le Y random en fonction de la taille du batiment
    vect2df_t vNewBuildingPos;
    vNewBuildingPos.x = 330;
    vNewBuildingPos.y = 160 - pBuildingSprSht->getSize().h + System::get()->getRandInt(0, 30);
    
    // TODO: Brancher ici la santé de la ville au lieu du random
    int iFrameId = System::get()->getRandInt(minStateId, maxStateId+1);
    
    Sprite* pNewBuildingSpr = new Sprite(pBuildingSprSht, iFrameId, vNewBuildingPos);
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

void ORCity::draw(uint8* buffer) {
    drawChildren(buffer);
}
