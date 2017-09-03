//
//  parallax-background.cpp
//  Muggine
//
//  Created by DiFFtY on 03/09/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "parallax-background.hpp"


ParallaxBackground::ParallaxBackground() {
    m_v2dCamPos.x = 0;
    m_v2dCamPos.y = 0;
}

ParallaxBackground::~ParallaxBackground() {
    
}

void ParallaxBackground::addLayer(Image* layerImg, int distance) {
    Layer* newLayer = new Layer;
    newLayer->img = layerImg;
    newLayer->distance = distance;
    newLayer->v2dPos.x = 0;
    newLayer->v2dPos.y = 0;
    
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
        
        currLayer->v2dPos.x = m_v2dCamPos.x / currLayer->distance;
        currLayer->v2dPos.y = m_v2dCamPos.y / currLayer->distance;
        
        currNode = currNode->pNext;
    }
}

void ParallaxBackground::draw(uint8* buffer) {
    LLNode* currNode = m_lLayers.pHead;
    
    while (currNode != NULL) {
        Layer* currLayer = (Layer*) currNode->pData;
        currLayer->img->draw(buffer, currLayer->v2dPos.x, currLayer->v2dPos.y, false, false);
        
        currNode = currNode->pNext;
    }
}
