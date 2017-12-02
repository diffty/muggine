#include "sprite.hpp"


Sprite::Sprite(Image* pImg, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y)
{
	m_pImg = pImg;
	m_pSprSht = NULL;

	size2df_t imgSize = m_pImg->getSize();
	m_rect.setSize(imgSize.w, imgSize.h);
}

Sprite::Sprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y)
{
	m_pImg = NULL;
	m_pSprSht = pSprSht;
	m_uFrameNb = uFrameNb;

	size2d_t imgSize = m_pSprSht->getFrameSize();
	m_rect.setSize(imgSize.w, imgSize.h);
}

Sprite::Sprite(uint rscId, RscManager* rscManager, float x, float y)
	: IWidget(x, y)
{
	m_rscId = rscId;
	m_pImg = rscManager->getImgRsc(m_rscId);
	size2df_t imgSize = m_pImg->getSize();
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
