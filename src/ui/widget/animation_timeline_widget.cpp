#include "animation_timeline_widget.hpp"

#include "../../core/system.hpp"


AnimationTimelineWidget::AnimationTimelineWidget(int x, int y, int w, int h, AnimationTimeline* pDrivenTimeline, float fMinTime, float fMaxTime)
	: IWidget(x, y, w, h) {

	m_pDrivenTimeline = pDrivenTimeline;
	m_fMinTime = fMinTime;
	m_fMaxTime = fMaxTime;
}

void AnimationTimelineWidget::onPress() {
	if (m_bIsPressed) {
		return;
	}

	m_bIsPressed = true;
}

void AnimationTimelineWidget::onRelease() {
	m_bIsPressed = false;
}

void AnimationTimelineWidget::update() {
	updateChildren();

	if (m_bIsPressed) {
		Input* pInputSys = System::get()->getInputSys();
		vect2d_t vMousePos = pInputSys->getCurrInputPos();

		vect2df_t vPos = m_rect.getPos();
		size2df_t sSize = m_rect.getSize();

		m_pDrivenTimeline->setTime(m_fMinTime + ((vMousePos.x - vPos.x) / sSize.w) * (m_fMaxTime - m_fMinTime));
		
		if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
			onRelease();
		}
	}

	m_bPressedThisLoop = false;
}

bool AnimationTimelineWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;

		return true;
	}
	else {
		return false;
	}
}

void AnimationTimelineWidget::draw(drawbuffer* pBuffer) {
	if (m_bIsActive) {
		drawChildren(pBuffer);

		vect2df_t vPos = m_rect.getPos();
		size2df_t sSize = m_rect.getSize();

		Color lineColor(255, 0, 0);
		drawLine(pBuffer, vPos.x, vPos.y+2, vPos.x + sSize.w, vPos.y+2, &lineColor);
		
		float fXPos = (m_pDrivenTimeline->getTime() - m_fMinTime) / (m_fMaxTime - m_fMinTime);
		if (fXPos < 0.) {
			fXPos = 0.;
		}
		else if (fXPos > 1.) {
			fXPos = 1.;
		}

		drawLine(pBuffer, vPos.x + fXPos * sSize.w, vPos.y - TIME_MARKER_HEIGHT + 4, vPos.x + fXPos * sSize.w, vPos.y + (TIME_MARKER_HEIGHT + 1) + 4, &lineColor);
	}
}
