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


CTTrain::CTTrain() {
    m_classInfo.setClassTypeName("CTTrain");
    m_pTransitionWagonImg = RscManager::get()->getImgRsc("transition-wagon");
}

CTTrain::~CTTrain() {
    
}

void CTTrain::update() {
    LLNode* pCurrNode = m_llChildrenWidgets.pHead;
    LLNode* pLastNode = m_llChildrenWidgets.pTail;
    
    while (pCurrNode != NULL) {
        IWidget* currTrainPart = (IWidget *) pCurrNode->pData;
        IWidget* lastTrainPart = (IWidget *) pLastNode->pData;
        
        vect2df_t trainPartPos = currTrainPart->getRect()->getPos();
        size2df_t trainPartSize = currTrainPart->getRect()->getSize();
        
        if (trainPartPos.y + trainPartSize.h < 0) {
            pCurrNode = pCurrNode->pNext;
            removeChildWidget(currTrainPart);
            
            if (strcmp(lastTrainPart->getClassType()->getClassTypeName(), "CTWagon") == 0) {
                addTransition();
            }
            else {
                addWagon();
            }

            delete currTrainPart;
        }
        else {
            break;
        }
    }
    
    updateChildren();
}

void CTTrain::addTransition() {
    Sprite* newPartObj = new Sprite(m_pTransitionWagonImg, 0, 0);
    
    LLNode* pLastNode = m_llChildrenWidgets.pTail;
    if (pLastNode != NULL) {
        IWidget* lastObject = (IWidget*) pLastNode->pData;
        
        vect2df_t vLastPartPos = lastObject->getRect()->getPos();
        size2df_t sLastPartSize = lastObject->getRect()->getSize();

        newPartObj->translate(vLastPartPos.x, (int) (vLastPartPos.y + sLastPartSize.h - 27), TRANSFORM_ABS);
    }

    addChildWidget(newPartObj);
}

void CTTrain::addWagon() {
    CTWagon* newPartObj = new CTWagon();
    
    LLNode* pLastNode = m_llChildrenWidgets.pTail;
    if (pLastNode != NULL) {
        IWidget* lastObject = (IWidget*) pLastNode->pData;
        
        vect2df_t vLastPartPos = lastObject->getRect()->getPos();
        size2df_t sLastPartSize = lastObject->getRect()->getSize();

        newPartObj->translate(vLastPartPos.x, (int) (vLastPartPos.y + sLastPartSize.h - 27), TRANSFORM_ABS);
    }

    addChildWidget(newPartObj);
}

void CTTrain::destroyWagon() {
    LLNode* pNode = m_llChildrenWidgets.pHead;
    if (pNode != NULL) {
        IWidget* headWidget = (IWidget*) pNode->pData;
        removeChildWidget(headWidget);
        delete headWidget;
    }
}

void CTTrain::draw(drawbuffer* pBuffer) {
    drawChildren(pBuffer);
}
