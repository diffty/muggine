#include "sprite.hpp"

Sprite::Sprite(uint rscId, RscManager* rscManager, int x, int y)
	: IWidget(x, y)
{
	m_rscId = rscId;
	m_image = rscManager->getImgResource(m_rscId);
	size2d_t imgSize = m_image->getSize();
	m_rect.setSize(imgSize.w, imgSize.h);
}

void Sprite::draw(uint8* buffer) {
    if (m_bIsActive) {
        vect2d_t pos = m_rect.getPos();
        m_image->draw(buffer, pos.x, pos.y, false, false);
    }
}

void Sprite::translate(int x, int y, ETransformMode transformMode) {
	vect2d_t currPos = m_rect.getPos();
	if (transformMode == TRANSFORM_REL) {
		currPos.x += x;
		currPos.y += y;
	}
	else {
		currPos.x = x;
		currPos.y = y;
	}
	m_rect.setPos(currPos.x, currPos.y);
}

void Sprite::setActive(bool bIsActive) {
	m_bIsActive = bIsActive;
}

bool Sprite::isActive() {
    return m_bIsActive;
}
