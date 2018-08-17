//
//  jnl_app_manager.cpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "jnl_app_manager.hpp"


JournalAppManager::JournalAppManager() {
    m_pCurrScene = new Scene();
    
    m_pCurrScene->addComponent(new CalendarView(10, 10, 100, 100));
    m_pCurrScene->addComponent(new DayContentPane());
}

JournalAppManager::~JournalAppManager() {
    
}

Scene* JournalAppManager::getScene() {
    return m_pCurrScene;
}

void JournalAppManager::update() {
    m_pCurrScene->update();
}

void JournalAppManager::draw(uint8* buffer) {
    m_pCurrScene->draw(buffer);
}

void JournalAppManager::receiveTouchInput(vect2d_t inputPos) {
    m_pCurrScene->receiveTouchInput(inputPos);
}
