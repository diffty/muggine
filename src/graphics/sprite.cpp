#include "sprite.hpp"


Sprite::Sprite(DrawBuffer* pDrawBuff, vect2df_t vPos, char* szName) :
    IWidget(vPos.x, vPos.y, szName)
{
    init(pDrawBuff, vPos.x, vPos.y);
}

Sprite::Sprite(DrawBuffer* pDrawBuff, float fXPos, float fYPos, char* szName) :
    IWidget(fXPos, fYPos, szName)
{
    init(pDrawBuff, fXPos, fYPos);
}

Sprite::Sprite(Image* pImg, vect2df_t vPos, char* szName) :
    IWidget(vPos.x, vPos.y, szName)
{
    init(pImg, vPos.x, vPos.y);
}

Sprite::Sprite(Image* pImg, float fXPos, float fYPos, char* szName) :
    IWidget(fXPos, fYPos, szName)
{
    init(pImg, fXPos, fYPos);
}

Sprite::Sprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, char* szName) :
    IWidget(vPos.x, vPos.y, szName)
{
    init(pSprSht, uFrameNb, vPos.x, vPos.y);
}

Sprite::Sprite(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos, char* szName) :
    IWidget(fXPos, fYPos, szName)
{
    init(pSprSht, uFrameNb, fXPos, fYPos);
}

void Sprite::init(DrawBuffer* pDrawBuf, float fXPos, float fYPos) {
    getClassType()->setClassTypeName("Sprite");
    
    m_pDrawBuf = pDrawBuf;
    m_pImg = NULL;
    m_pSprSht = NULL;
    
    size2df_t imgSize = m_pDrawBuf->getSize();
    m_rect.setSize(imgSize.w, imgSize.h);
}

void Sprite::init(Image* pImg, float fXPos, float fYPos) {
    getClassType()->setClassTypeName("Sprite");

    m_pDrawBuf = NULL;
    m_pImg = pImg;
    m_pSprSht = NULL;
    
    size2df_t imgSize = m_pImg->getSize();
    m_rect.setSize(imgSize.w, imgSize.h);
}

void Sprite::init(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos) {
    getClassType()->setClassTypeName("Sprite");

    m_pDrawBuf = NULL;
    m_pImg = NULL;
    m_pSprSht = pSprSht;
    m_uFrameNb = uFrameNb;
    
    size2d_t imgSize = m_pSprSht->getFrameSize();
    m_rect.setSize(imgSize.w, imgSize.h);
}

uint Sprite::getFrame() {
    return m_uFrameNb;
}

void Sprite::setFrame(uint uNewFrame) {
    m_uFrameNb = uNewFrame;
}

void Sprite::draw(drawbuffer* pBuffer) {
    // TODO: faire ce test a l'echelle de l'objet Scene qui va appeler les draw et update
    if (m_bIsActive) {
        vect2df_t pos = m_rect.getPos();

        if (m_pDrawBuf)
            m_pDrawBuf->draw(pBuffer, pos.x, pos.y, false, true);
        else if (m_pSprSht)
            m_pSprSht->draw(pBuffer, m_uFrameNb, pos.x, pos.y, false, true);
        else
            m_pImg->draw(pBuffer, pos.x, pos.y, false, true);
    
        drawChildren(pBuffer);
    }
}
