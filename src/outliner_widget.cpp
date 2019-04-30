#include "outliner_widget.hpp"

#include "system.hpp"


OutlinerWidget::OutlinerWidget(int x, int y, int w, int h, Scene* pRootScene)
	: IWidget(x, y, w, h) {
}

void OutlinerWidget::onPress() {
	if (m_bIsPressed) {
		return;
	}

	m_bIsPressed = true;
}

void OutlinerWidget::onRelease() {
	m_bIsPressed = false;
}

void OutlinerWidget::update() {
	updateChildren();

	if (m_bIsPressed) {
		Input* pInputSys = System::get()->getInputSys();
		vect2d_t vMousePos = pInputSys->getCurrInputPos();

		vect2df_t vPos = m_rect.getPos();
		size2df_t sSize = m_rect.getSize();

		if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
			onRelease();
		}
	}

	m_bPressedThisLoop = false;
}

bool OutlinerWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;

		return true;
	}
	else {
		return false;
	}
}

void OutlinerWidget::draw(uint8* buffer) {
	if (m_bIsActive) {
		drawChildren(buffer);
	}
}
