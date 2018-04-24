//
//  or_pickup_items_manager.cpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_pickup_items_manager.hpp"
#include "or_game_mode.hpp"


ORPickupItemsManager::ORPickupItemsManager() {
    initList(&m_llPickupList);
    
    m_fTimeBeforeNextItemSpawn = System::get()->getRandFloat(MIN_SPAWN_TIME, MAX_SPAWN_TIME);
    printf("%f\n", m_fTimeBeforeNextItemSpawn);
}

ORPickupItemsManager::~ORPickupItemsManager() {
    clearList(&m_llPickupList);
}

void ORPickupItemsManager::spawnItem() {
    int maxItemId = 3;
    
    if (ORGameMode::get()->getPopulationValue() > 100) {
        maxItemId = 4;
    }
    
    int iItemTypeId = System::get()->getRandInt(0, maxItemId);
    
    ORPickupItem* pNewPickupObj = NULL;
    
    switch(iItemTypeId) {
        case 0:
            pNewPickupObj = (ORPickupItem*) new ORPickupItemHouse();
            break;
            
        case 1:
            pNewPickupObj = (ORPickupItem*) new ORPickupItemIndustry();
            break;
            
        case 2:
            pNewPickupObj = (ORPickupItem*) new ORPickupItemEcology();
            break;
            
        case 3:
            pNewPickupObj = (ORPickupItem*) new ORPickupItemTomato();
            break;
    }
    
    if (pNewPickupObj != NULL) {
        pNewPickupObj->translate(325, 150 + System::get()->getRandInt(0, 3) * 16);
        addChildWidget(pNewPickupObj);
        
        addDataToList(&m_llPickupList, pNewPickupObj);
    }
}

LinkedList* ORPickupItemsManager::getItemsList() {
    return &m_llPickupList;
}

void ORPickupItemsManager::onItemPicked(ORPickupItem* pPickedUpItem) {
    LLNode* pFoundNode = searchForNodeInList(&m_llPickupList, pPickedUpItem);
    removeNodeFromList(&m_llPickupList, pFoundNode);
    delete pFoundNode;
    removeChildWidget(pPickedUpItem);
    ORGameMode::get()->destroyWidget(pPickedUpItem);
}

void ORPickupItemsManager::update() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    updateChildren();
    
    LLNode* pCurrNode = m_llPickupList.pHead;
    
    while (pCurrNode != NULL) {
        ORPickupItem* pCurrItem = (ORPickupItem*)pCurrNode->pData;
        pCurrItem->translate(-ORGameMode::get()->getRoadScrollSpeed() * fDeltaTime, 0, TRANSFORM_REL);
        pCurrNode = pCurrNode->pNext;
    }
    
    if (m_fTimeBeforeNextItemSpawn < 0.0) {
        spawnItem();
        m_fTimeBeforeNextItemSpawn = System::get()->getRandFloat(MIN_SPAWN_TIME, MAX_SPAWN_TIME);
    }
    else {
        m_fTimeBeforeNextItemSpawn -= System::get()->getDeltaTime();
    }
}

void ORPickupItemsManager::draw(uint8* buffer) {
    drawChildren(buffer);
}
