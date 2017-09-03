//
//  parallax-background.cpp
//  Muggine
//
//  Created by DiFFtY on 03/09/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "parallax_background.hpp"


ParallaxBackground::ParallaxBackground() {
    m_v2dCamPos.x = 0;
    m_v2dCamPos.y = 0;
    
    initList(&m_lLayers);
}

ParallaxBackground::~ParallaxBackground() {
    
}

void ParallaxBackground::addLayer(Image* layerImg, int distance, int offX, int offY) {
    Layer* newLayer = new Layer;
    newLayer->img = layerImg;
    newLayer->distance = distance;
    newLayer->v2dPos.x = 0;
    newLayer->v2dPos.y = 0;
    newLayer->offset.x = offX;
    newLayer->offset.y = offY;
    
    LLNode* newNode = new LLNode;
    newNode->pData = newLayer;
    addNodeToList(&m_lLayers, newNode);
}

void ParallaxBackground::deleteLayer(int layerNum) {
    LLNode* nodeToDelete = removeNodeFromList(&m_lLayers, getNodeInList(&m_lLayers, layerNum));
    Layer* layerToDelete = (Layer *) nodeToDelete->pData;
    
    delete layerToDelete;
    delete nodeToDelete;
}

void ParallaxBackground::update() {
    LLNode* currNode = m_lLayers.pHead;
    
    while (currNode != NULL) {
        Layer* currLayer = (Layer*) currNode->pData;
        
        if (currLayer->distance == 0) {
            currLayer->v2dPos.x = m_v2dCamPos.x + currLayer->offset.x;
            currLayer->v2dPos.y = m_v2dCamPos.y + currLayer->offset.y;
        }
        else {
            currLayer->v2dPos.x = m_v2dCamPos.x / currLayer->distance + currLayer->offset.x;
            currLayer->v2dPos.y = m_v2dCamPos.y / currLayer->distance + currLayer->offset.y;
        }
        
        while (currLayer->v2dPos.x > 0) {
            currLayer->v2dPos.x -= currLayer->img->getSize().w;
        }
        
        while (currLayer->v2dPos.x < 0) {
            currLayer->v2dPos.x += currLayer->img->getSize().w;
        }
        
        currNode = currNode->pNext;
    }
}

void ParallaxBackground::draw(uint8* buffer) {
    LLNode* currNode = m_lLayers.pHead;
    
    while (currNode != NULL) {
        Layer* currLayer = (Layer*) currNode->pData;
        
        int currXPos = currLayer->v2dPos.x;
        
        while (currXPos + currLayer->img->getSize().w > 0) {
            currXPos -= currLayer->img->getSize().w;
        }
        
        do {
            currXPos += currLayer->img->getSize().w;

            currLayer->img->draw(buffer, m_rect.getPos().x + currXPos, m_rect.getPos().y + currLayer->v2dPos.y, false, false);
            
        } while (m_rect.getPos().x + currXPos < SCREEN_WIDTH);

        currNode = currNode->pNext;
    }
}

void ParallaxBackground::setCamPos(int x, int y) {
    m_v2dCamPos.x = x;
    m_v2dCamPos.y = y;
}

vect2d_t ParallaxBackground::getCamPos() {
    return m_v2dCamPos;
}
