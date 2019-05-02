#include "titlebar.hpp"

#include "../manager/rsc_manager.hpp"
#include "../graphics/drawing.hpp"
#include "../core/system.hpp"


Titlebar::Titlebar(char* szTitle, IWidget* pParentObject)
	: IWidget(0, 0),
	m_color(20, 20, 20) {

	size2df_t sParentSize = pParentObject->getRect()->getSize();
	getRect()->setSize(sParentSize.w, UI_TITLEBAR_HEIGHT);
}

Titlebar::~Titlebar() {

}

void Titlebar::addButton(SpriteSheet *pSprSht, vect2df_t vPos, int iPressedFrameId, int iReleasedFrameId, void(*pCallback)(void*), void* pCallbackArg) {

	ImageButtonWidget* pCloseButtonWidget = new ImageButtonWidget(
		pSprSht,
		{ getRect()->getPos().x + vPos.x, getRect()->getPos().y + vPos.y },
		iPressedFrameId,
		iReleasedFrameId,
		0,
		pCallback,
		(void*)pCallbackArg);

	pCloseButtonWidget->setParentWidget(this);
}

void Titlebar::onPress() {
	if (m_state == 2 || m_state == 4) {
		return;
	}

	m_state = (m_state == 0) ? 2 : 4;

	m_vPrevCursorPosition = System::get()->getInputSys()->getCurrInputPos();
}

void Titlebar::onRelease() {
	m_state = 0;
}

bool Titlebar::receiveTouchInput(vect2d_t touchPt) {
	printf("%i; %i\n", touchPt.x, touchPt.y);
	bool touchTest = receiveTouchInputChildren(touchPt);

	if (!touchTest) {
		if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
			onPress();
			m_bPressedThisLoop = true;
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

void Titlebar::update() {
	updateChildren();
	if (m_bPressedThisLoop) {
		if (System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
			onMove();
		}
		else {
			m_bPressedThisLoop = false;
			onRelease();
		}
	}
}

void Titlebar::draw(uint8* pBuffer) {
	vect2df_t vPos = m_rect.getPos();
	size2df_t sSize = m_rect.getSize();
	drawBox(pBuffer, vPos.x, vPos.y, vPos.x + sSize.w, vPos.y + sSize.h, &m_color);
	drawChildren(pBuffer);
}

void Titlebar::onMove() {
	Input* pInputSys = System::get()->getInputSys();
	vect2d_t vCurrCursorPosition = pInputSys->getCurrInputPos();
	vect2d_t vDeltaPos = {
		vCurrCursorPosition.x - m_vPrevCursorPosition.x,
		vCurrCursorPosition.y - m_vPrevCursorPosition.y,
	};
	m_vPrevCursorPosition = vCurrCursorPosition;
	m_pParentWidget->translate(vDeltaPos.x, vDeltaPos.y, TRANSFORM_REL);
}
