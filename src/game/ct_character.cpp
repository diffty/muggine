//
//  ct_character.cpp
//  DeMuGGinE
//
//  Created by DiFFtY on 16/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#include "ct_character.hpp"

#include "../manager/rsc_manager.hpp"
#include "../graphics/spritesheet.hpp"
#include "../core/system.hpp"
#include "../graphics/drawing.hpp"


CTCharacter::CTCharacter() {
    m_classInfo.setClassTypeName("CTCharacter");
    
    RscManager* rscManager = RscManager::get();
    
    SpriteSheet* facesImg = rscManager->getSprShtRsc("faces");
    SpriteSheet* manHaircutsImg = rscManager->getSprShtRsc("man-haircuts");
    SpriteSheet* womanHaircutsImg = rscManager->getSprShtRsc("woman-haircuts");
    SpriteSheet* clothesImg = rscManager->getSprShtRsc("clothes");
    
    // Initing chara
    m_pDrawBuffer = new DrawBuffer();
    m_pDrawBuffer->createBuffer(18, 40);
    size2d_t drawBufSize = m_pDrawBuffer->getSizei();
    
    Color bgClr(0, 255, 0);
    
    // Clearing chara's buffer background with transparent color
    drawBox(m_pDrawBuffer->getBuffer(), 0, 0, drawBufSize.w-1, drawBufSize.h-1, &bgClr);
    
    System* pSys = System::get();
    
    uint clothesFrame = pSys->getRandInt(0, clothesImg->getLength());
    uint facesFrame = pSys->getRandInt(0, facesImg->getLength());
    uint manHaircutsFrame = pSys->getRandInt(0, manHaircutsImg->getLength());

    clothesImg->draw(m_pDrawBuffer->getBuffer(), clothesFrame, 3, 14, false, true);
    facesImg->draw(m_pDrawBuffer->getBuffer(), facesFrame, 3, 2, false, true);
    manHaircutsImg->draw(m_pDrawBuffer->getBuffer(), manHaircutsFrame, 3, 2, false, true);

    m_pDrawBuffer->detectTransparency(bgClr.getColor());
}

CTCharacter::~CTCharacter() {
    delete m_pDrawBuffer;
}

DrawBuffer* CTCharacter::getDrawBuffer() {
    return m_pDrawBuffer;
}

void CTCharacter::draw(drawbuffer* pBuffer) {
    if (m_bIsActive) {
        vect2df_t pos = m_rect.getPos();
        m_pDrawBuffer->draw(pBuffer, pos.x, pos.y, false, true);
    }
    drawChildren(pBuffer);
}
