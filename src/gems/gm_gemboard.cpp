//
//  gm_gemboard.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 06/10/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "gm_gemboard.hpp"


GemBoard::GemBoard(int iGridW, int iGridH, SpriteSheet* pGemSprSht) {
    initList(&m_llGemDestroyList);

    m_iGridW = iGridW;
    m_iGridH = iGridH;
    
    m_pGemSprSht = pGemSprSht;
    
    int iNbGems = m_iGridW * m_iGridH;
    
    m_gemGrid = new Gem*[iNbGems];
    m_gemReserveGrid = new Gem*[iNbGems];
    
    for (int i = 0; i < iNbGems; i++) {
        m_gemGrid[i] = NULL;
        m_gemReserveGrid[i] = NULL;
    }
    
    generateGrid();
}

GemBoard::~GemBoard() {
    // lol jsp si faut libérer l'espace ou pas :v
    /*for (int i = 0; i < m_iGridW * m_iGridH; i++) {
        removeChildWidget(m_gemGrid[i]);
        delete m_gemGrid[i];
    }*/
    delete m_gemGrid;
    delete m_gemReserveGrid;
}

void GemBoard::generateGrid() {
    int iNbGems = m_iGridW * m_iGridH;
    
    for (int i = 0; i < iNbGems; i++) {
        Gem* pNewGem = new Gem(m_pGemSprSht);
        
        m_gemGrid[i] = pNewGem;
        pNewGem->setParentWidget(this);
        
        int x = i % m_iGridW;
        int y = i / m_iGridW;
        
        pNewGem->translate(x * 18, y * 18);
    }
}

void GemBoard::generateNewGems(int rowId, int nbGemsToGenerate) {
    printf("Generating %i gems at %i\n", nbGemsToGenerate, rowId);
    for (int y = 0; y < nbGemsToGenerate; y++) {
        Gem* pNewGem = new Gem(m_pGemSprSht);
        
        pNewGem->setParentWidget(this);
        pNewGem->translate(rowId * 18, -((nbGemsToGenerate-1-y)+1) * 18);
    
        vect2d_t realPos;
        realPos.x = rowId * 18;
        realPos.y = y * 18;

        pNewGem->moveTo(realPos);
        
        int iNewGemId = rowId + (y * m_iGridW);
        m_gemGrid[iNewGemId] = pNewGem;
    }
}

void GemBoard::destroyGem(Gem* pGem) {
    addDataToList(&m_llGemDestroyList, (void*) pGem);
}

void GemBoard::destroyAllGemsOfColor(int iColorId) {
    for (int i = 0; i < m_iGridW * m_iGridH; i++) {
        Gem* pGem = m_gemGrid[i];
        if (pGem->getColorId() == iColorId) {
            destroyGem(pGem);
        }
    }
}

void GemBoard::purgeGemTrash() {
    LLNode* pCurrNode = m_llGemDestroyList.pHead;
    
    while (pCurrNode != NULL) {
        Gem* pGem = (Gem*) pCurrNode->pData;
        vect2d_t gemGridCoord = this->findGemGridCoord(pGem);
        removeNodeFromList(&m_llGemDestroyList, pCurrNode);
        LLNode* pNextNode = pCurrNode->pNext;
        delete pCurrNode;
        delete pGem;
        pCurrNode = pNextNode;
        int iGridId = gemGridCoord.x + gemGridCoord.y * m_iGridW;
        m_gemGrid[iGridId] = NULL;
        onGemDestruction(gemGridCoord);
    }
}

vect2d_t GemBoard::findGemGridCoord(Gem* pGem) {
    vect2d_t vCoord;
    vCoord.x = -1;
    vCoord.y = -1;
    
    for (int i = 0; i < m_iGridW * m_iGridH; i++) {
        if (m_gemGrid[i] == pGem) {
            vCoord.x = i % m_iGridW;
            vCoord.y = i / m_iGridW;
            return vCoord;
        }
    }
    
    return vCoord;
}

void GemBoard::onGemDestruction(vect2d_t vGemGridCoord) {
    printf("%d, %d\n", vGemGridCoord.x, vGemGridCoord.y);
    int x = vGemGridCoord.x;
    
    vect2d_t boardPos = getRect()->getPosi();
    
    vect2d_t realPos;
    realPos.x = x * 18;
    
    int gridFallY = 0;
    
    for (int y = vGemGridCoord.y; y >= 0; y--) {
        int iGemId = x + y * m_iGridW;

        Gem* pCurrGem = m_gemGrid[iGemId];
        
        if (pCurrGem != NULL && gridFallY > 0) {
            realPos.y = (y + gridFallY) * 18;

            pCurrGem->moveTo(realPos);
            int iNewGemId = x + (y + gridFallY) * m_iGridW;
            m_gemGrid[iGemId] = NULL;
            m_gemGrid[iNewGemId] = pCurrGem;
        }
        else {
            gridFallY++;
        }
    }
    generateNewGems(vGemGridCoord.x, gridFallY);
}

void GemBoard::draw(drawbuffer* pBuffer) {
    drawChildren(pBuffer);
}

void GemBoard::update() {
    updateChildren();
    purgeGemTrash();
}

bool GemBoard::receiveTouchInput(vect2d_t touchPt) {
    return receiveTouchInputChildren(touchPt);
}
