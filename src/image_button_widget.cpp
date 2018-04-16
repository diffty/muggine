#include "image_button_widget.hpp"


ImageButtonWidget::ImageButtonWidget(SpriteSheet *pSprSht, vect2df_t vPos, int iPressedFrameId, int iReleasedFrameId, int initalState, void(*pCallback)(void*), void* pCallbackArg)
	: Sprite(pSprSht, iReleasedFrameId, vPos) {

	m_iPressedFrameId = iPressedFrameId;
	m_iReleasedFrameId = iReleasedFrameId;

	m_state = initalState;
	m_bPressedThisLoop = false;
	m_mode = E_BTNMODE_INSTANT;

	m_pOnPressCallback = pCallback;
	m_pOnPressCallbackArg = pCallbackArg;
}

void ImageButtonWidget::setOnPressCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pOnPressCallback = pCallback;
	m_pOnPressCallbackArg = pCallbackArg;
}

void ImageButtonWidget::setOnReleaseCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pOnReleaseCallback = pCallback;
	m_pOnReleaseCallbackArg = pCallbackArg;
}

void ImageButtonWidget::onPress() {
	if (m_state == 2 || m_state == 4) {
		return;
	}

	m_state = (m_state == 0) ? 2 : 4;

	setFrame(m_iPressedFrameId);

	if (m_pOnPressCallback != NULL)
		(*m_pOnPressCallback)(m_pOnPressCallbackArg);

}

void ImageButtonWidget::onRelease() {
	if (m_mode == E_BTNMODE_TOGGLE && m_state == 2) {
		m_state = 1;
	}
	else if (m_mode == E_BTNMODE_INSTANT || m_state == 4) {
		m_state = 0;
		setFrame(m_iReleasedFrameId);
	}

	if (m_pOnReleaseCallback != NULL)
		(*m_pOnReleaseCallback)(m_pOnReleaseCallbackArg);
}

void ImageButtonWidget::update() {
	updateChildren();

	if (!m_bPressedThisLoop && (m_state == 2 || m_state == 4)) {
		onRelease();
	}
	m_bPressedThisLoop = false;
}

bool ImageButtonWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_bIsActive && m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;
		return true;
	}
	return false;
}
