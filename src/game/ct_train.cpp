//
//  ct_train.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 25/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "ct_train.hpp"

#include "../core/common_types.hpp"
#include "../graphics/sprite.hpp"
#include "../game/ct_wagon.hpp"
#include "../manager/rsc_manager.hpp"
#include "../game/ct_door.hpp"
#include "../graphics/spritesheet.hpp"
#include <cmath>


CTTrain::CTTrain() {
    m_pTrainScene = new Scene();
    m_pWallScene = new Scene();
    
    m_classInfo.setClassTypeName("CTTrain");
    m_pTransitionWagonImg = RscManager::get()->getImgRsc("transition-wagon");
    
    SpriteSheet* chaSprSht = RscManager::get()->getSprShtRsc("cha");
    SpriteSheet* fredSprSht = RscManager::get()->getSprShtRsc("fred");
    
    m_pCha = new AnimatedSprite(chaSprSht, 45, 70);
    m_pFred = new AnimatedSprite(fredSprSht, 45, 45);
    
    m_pCha->addState("idle", 0, 0, 1, false);
    m_pCha->addState("walking", 1, 4, 4, true);
    
    m_pFred->addState("idle", 0, 0, 1, false);
    m_pFred->addState("walking", 1, 4, 4, true);
    
    m_pCha->changeState(1);
    m_pFred->changeState(1);
    
    m_pCha->play();
    m_pFred->play();
    
    addChildWidget(m_pTrainScene);
    addChildWidget(m_pCha);
    addChildWidget(m_pFred);
    addChildWidget(m_pWallScene);
    
    m_fPauseTime = -1;
}

CTTrain::~CTTrain() {
    
}

void CTTrain::update() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    int nbChildren = m_pTrainScene->countChildren();

    IWidget* lastTrainPart = m_pTrainScene->getLastChild();
    
    for (int i = 0; i < nbChildren; i++) {
        IWidget* currTrainPart = m_pTrainScene->getChild(i);
        
        vect2df_t trainPartPos = currTrainPart->getRect()->getPos();
        size2df_t trainPartSize = currTrainPart->getRect()->getSize();
        
        if (trainPartPos.y + trainPartSize.h < 0) {
            m_pTrainScene->removeChildWidget(currTrainPart);
            m_pWallScene->removeChildWidget(m_pWallScene->getChild(0));
            
            i--;
            
            if (strcmp(lastTrainPart->getClassType()->getClassTypeName(), "CTWagon") == 0) {
                addTransition();
            }
            else {
                addWagon();
            }

            delete currTrainPart;
            
            nbChildren = m_pTrainScene->countChildren();
        }
        else {
            break;
        }
    }
    
    CTDoor* pDoor = isInFrontOfDoor();

    if (pDoor != NULL && !pDoor->isOpened() && m_fPauseTime < 0) {
        m_pCha->changeState(0);
        m_pFred->changeState(0);
        
        m_fPauseTime = 1.;

        pDoor->changeState(1);
        pDoor->play();
    }
    
    if (m_pCha->getStateId() == 1) {
        m_pCha->translate(0, 25. * fDeltaTime);
    }
    
    if (m_pFred->getStateId() == 1) {
        m_pFred->translate(0, 25. * fDeltaTime);
    }
    
    if (m_fPauseTime > 0) {
        m_fPauseTime -= System::get()->getDeltaTime();
        
        if (m_fPauseTime <= 0) {
            m_pCha->changeState(1);
            m_pFred->changeState(1);
        }
    }

    translate(0, -m_fSpeed * fDeltaTime, TRANSFORM_REL);

    updateChildren();
}

void CTTrain::addTransition() {
    Sprite* newSpriteObj = new Sprite(m_pTransitionWagonImg, 0, 0);
    addPart(newSpriteObj);
}

void CTTrain::addWagon() {
    CTWagon* newWagonObj = new CTWagon();
    addPart(newWagonObj);
}

void CTTrain::addPart(IWidget* pNewPart) {
    IWidget* pLastObject = m_pTrainScene->getLastChild();
    
    if (pLastObject != NULL) {
        vect2df_t vLastPartPos = pLastObject->getRect()->getPos();
        size2df_t sLastPartSize = pLastObject->getRect()->getSize();

        pNewPart->translate(vLastPartPos.x, (int) (vLastPartPos.y + sLastPartSize.h - 27), TRANSFORM_ABS);
    }

    m_pTrainScene->addChildWidget(pNewPart);
    addDoorForPart(pNewPart);
    addWallForPart(pNewPart);
}

Scene* CTTrain::getWallScene() {
    return m_pWallScene;
}

CTDoor* CTTrain::addDoorForPart(IWidget* pTrainPart) {
    vect2df_t trainPartPos = pTrainPart->getRect()->getPos();
    size2df_t trainPartSize = pTrainPart->getRect()->getSize();
    CTDoor* door = new CTDoor(trainPartPos.x + trainPartSize.w / 2 - 10, trainPartPos.y + 5);
    m_pWallScene->addChildWidget(door);
    return door;
}

Sprite* CTTrain::addWallForPart(IWidget* pTrainPart) {
    vect2df_t trainPartPos = pTrainPart->getRect()->getPos();
    size2df_t trainPartSize = pTrainPart->getRect()->getSize();
    Sprite* wall = new Sprite(RscManager::get()->getImgRsc("transition-wagon-wall"), trainPartPos.x + trainPartSize.w / 2 - 49, trainPartPos.y + 0);
    m_pWallScene->addChildWidget(wall);
    return wall;
}

CTDoor* CTTrain::isInFrontOfDoor() {
    for (int i = 0; i < m_pWallScene->countChildren(); i++) {
        IWidget* pCurrWidget = m_pWallScene->getChild(i);
        
        if (strcmp(pCurrWidget->getClassType()->getClassTypeName(), "CTDoor") == 0) {
            CTDoor* pCurrDoor = (CTDoor*) pCurrWidget;
            
            vect2df_t vDoorPos = pCurrWidget->getRect()->getPos();
            vect2df_t vChaPos = m_pCha->getRect()->getPos();
            
            if (abs(vDoorPos.y - vChaPos.y) < 10) {
                return pCurrDoor;
            }
        }
    }
    return NULL;
}

void CTTrain::draw(drawbuffer* pBuffer) {
    drawChildren(pBuffer);
}
