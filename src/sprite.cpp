#include "sprite.hpp"

Sprite::Sprite(uint rscId, RscManager* rscManager, int x, int y)
	: IWidget(x, y)
{
	m_rscId = rscId;
	m_image = rscManager->getImgRsc(m_rscId);
	size2d_t imgSize = m_image->getSize();
	m_rect.setSize(imgSize.w, imgSize.h);
}

void Sprite::draw(uint8* buffer) {
    if (m_bIsActive) {
        vect2d_t pos = m_rect.getPos();
        m_image->draw(buffer, pos.x, pos.y, false, false);
    }
}
