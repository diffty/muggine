#include "sprite.hpp"


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

void Sprite::init(Image* pImg, float fXPos, float fYPos) {
    m_pImg = pImg;
    m_pSprSht = NULL;
    
    size2df_t imgSize = m_pImg->getSize();
    m_rect.setSize(imgSize.w, imgSize.h);
}

void Sprite::init(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos) {
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

void Sprite::draw(uint8* buffer) {
	// TODO: faire ce test à l'échelle de l'objet Scene qui va appeler les draw et update
    if (m_bIsActive) {
        vect2df_t pos = m_rect.getPos();

		if (m_pSprSht)
			m_pSprSht->draw(buffer, m_uFrameNb, pos.x, pos.y, false, true);
		else
			m_pImg->draw(buffer, pos.x, pos.y, false, true); 
	
		drawChildren(buffer);
	}
}
