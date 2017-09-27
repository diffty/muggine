#include "draggable_sprite.hpp"



DraggableSprite::DraggableSprite(Image* pImg, vect2df_t vPos, Input* pInputManager, bool bIsDraggable) :
	Sprite(pImg, vPos) {

	init(pInputManager, bIsDraggable);
}
DraggableSprite::DraggableSprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, Input* pInputManager, bool bIsDraggable) :
	Sprite(pSprSht, uFrameNb, vPos) {

	init(pInputManager, bIsDraggable);
}


DraggableSprite::~DraggableSprite() {

}

void DraggableSprite::init(Input* pInputManager, bool bIsDraggable) {
	m_pInputManager = pInputManager;
	m_bIsGrabbed = false;
	m_bIsDraggable = bIsDraggable;
}

void DraggableSprite::update() {
	if (m_bIsGrabbed) {
		if (m_pInputManager->IsButtonPressed(MOUSE_BTN_LEFT)) {
			// On dragging
			m_vCurrMousePos = m_pInputManager->getCurrInputPos();

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
}

void DraggableSprite::receiveTouchInput(vect2d_t vTouchPt) {
	if (m_bIsDraggable && !m_bIsGrabbed && m_rect.isPointInRect(vTouchPt.x, vTouchPt.y)) {
		// On start dragging
		m_bIsGrabbed = true;
		
		vect2df_t vCurrTouchPos = m_rect.getPos();

		m_vGrabPos.x = vTouchPt.x - vCurrTouchPos.x;
		m_vGrabPos.y = vTouchPt.y - vCurrTouchPos.y;

		onDragStart(vTouchPt);
	}

	m_vCurrMousePos = vTouchPt;
}

bool DraggableSprite::isDraggable() {
	return m_bIsDraggable;
}

void DraggableSprite::setDraggable(bool bIsDraggable) {
	m_bIsDraggable = bIsDraggable;
}