#include "buttonwidget.hpp"


ButtonWidget::ButtonWidget(int x, int y, int w, int h, int initalState, int (*pCallback)(void))
	: IWidget(x, y, w, h),
	  m_colorOn(255, 0, 3),
	  m_colorOff(3, 0, 255),
	  m_colorPressed(3, 255, 3) {

	m_state = initalState;
	m_bPressedThisLoop = false;
	m_mode = E_BTNMODE_INSTANT;
	m_pCallback = pCallback;
}

void ButtonWidget::onPress() {
	int i;

	if (m_state == 2 || m_state == 4) {
		return;
	}

	m_state = (m_state == 0) ? 2 : 4;

	(*m_pCallback)();
}

void ButtonWidget::onRelease() {
	if (m_mode == E_BTNMODE_TOGGLE && m_state == 2) {
		m_state = 1;
	}
	else if (m_mode == E_BTNMODE_INSTANT || m_state == 4) {
		m_state = 0;
	}
}

void ButtonWidget::update() {
	if (!m_bPressedThisLoop && (m_state == 2 || m_state == 4)) {
		onRelease();
	}
	m_bPressedThisLoop = false;
}

void ButtonWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;
	}
}

void ButtonWidget::draw(uint8* fb) {
	if (m_state == 1) {
		drawBox(fb,
			m_rect.getPos().x,
			m_rect.getPos().y,
			m_rect.getPos().x + m_rect.getSize().w,
			m_rect.getPos().y + m_rect.getSize().h,
			&m_colorOn
		);
	}
	else if (m_state == 2 || m_state == 4){
		drawBox(fb,
			m_rect.getPos().x,
			m_rect.getPos().y,
			m_rect.getPos().x + m_rect.getSize().w,
			m_rect.getPos().y + m_rect.getSize().h,
			&m_colorPressed
		);
	}
	else {
		drawBox(fb,
			m_rect.getPos().x,
			m_rect.getPos().y,
			m_rect.getPos().x + m_rect.getSize().w,
			m_rect.getPos().y + m_rect.getSize().h,
			&m_colorOff
		);
	}
}