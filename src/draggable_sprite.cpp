#include "draggable_sprite.hpp"
#include "system.hpp"


DraggableSprite::DraggableSprite(Image* pImg, vect2df_t vPos, bool bIsDraggable) :
	Sprite(pImg, vPos) {

	init(bIsDraggable);
}

DraggableSprite::DraggableSprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, bool bIsDraggable) :
	Sprite(pSprSht, uFrameNb, vPos) {

	init(bIsDraggable);
}


DraggableSprite::~DraggableSprite() {

}

void DraggableSprite::init(bool bIsDraggable) {
	m_bIsGrabbed = false;
	m_bIsDraggable = bIsDraggable;
	m_bIsHoverable = false;
	m_bIsHovered = false;
}

void DraggableSprite::update() {
	updateChildren();

	Input* pInputManager = System::get()->getInputSys();

	m_vCurrMousePos = pInputManager->getCurrInputPos();

	if (m_bIsGrabbed) {
		if (pInputManager->IsButtonPressed(MOUSE_BTN_LEFT)) {
			// On dragging
			
			translate(
				m_vCurrMousePos.x - m_vGrabPos.x,
				m_vCurrMousePos.y - m_vGrabPos.y, TRANSFORM_ABS
			);

			onDragging();
		}
		else {
			// On end dragging
			m_bIsGrabbed = false;

			onDragEnd();
		}
	}

	if (m_bIsHoverable) {
		// printf("%d\n", m_rect.isPointInRect(m_vCurrMousePos.x, m_vCurrMousePos.y));

		if (m_rect.isPointInRect(m_vCurrMousePos.x, m_vCurrMousePos.y)) {
			if (!m_bIsHovered) {
				m_bIsHovered = true;

				onHoverStart(m_vCurrMousePos);
			}
			else {
				onHovering();
			}
		}
		else if (m_bIsHovered) {
			m_bIsHovered = false;
			onHoverEnd();
		}
	}
}

bool DraggableSprite::receiveTouchInput(vect2d_t vTouchPt) {
	bool bIsHit = false;

	if (m_rect.isPointInRect(vTouchPt.x, vTouchPt.y)) {
		if (m_bIsDraggable && !m_bIsGrabbed) {
			// On start dragging
			m_bIsGrabbed = true;
		
			vect2df_t vCurrTouchPos = m_rect.getPos();

			m_vGrabPos.x = vTouchPt.x - vCurrTouchPos.x;
			m_vGrabPos.y = vTouchPt.y - vCurrTouchPos.y;

			onDragStart(vTouchPt);
			bIsHit = true;
		}
	}

	m_vCurrMousePos = vTouchPt;

	return bIsHit;
}

bool DraggableSprite::isDraggable() {
	return m_bIsDraggable;
}

void DraggableSprite::setDraggable(bool bIsDraggable) {
	m_bIsDraggable = bIsDraggable;
}