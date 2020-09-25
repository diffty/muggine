#include "tiny_progress_bar.hpp"



TinyProgressBar::TinyProgressBar(vect2df_t vPos, size2df_t sSize, float fMinValue, float fMaxValue, float fCurValue) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h)
{
	m_fMinValue = fMinValue;
	m_fMaxValue = fMaxValue;
	m_fCurValue = fCurValue;
}


TinyProgressBar::~TinyProgressBar() {

}

void TinyProgressBar::setValue(float fCurValue) {
	m_fCurValue = fCurValue;
}

void TinyProgressBar::setMinValue(float fMinValue) {
	m_fMinValue = fMinValue;
}

void TinyProgressBar::setMaxValue(float fMaxValue) {
	m_fMaxValue = fMaxValue;
}

void TinyProgressBar::draw(drawbuffer* pBuffer) {
	if (m_bIsActive) {
		drawChildren(pBuffer);

		vect2df_t vCurPos = m_rect.getPos();
		size2df_t sCurSize = m_rect.getSize();

		float completedBarSize = m_fCurValue / (m_fMaxValue - m_fMinValue) * sCurSize.w;

		Color completedColor(0, 255, 0);
		Color remainingColor(255, 0, 0);

		drawBox(pBuffer,
			vCurPos.x,
			vCurPos.y,
			vCurPos.x + completedBarSize,
			vCurPos.y + sCurSize.h,
			&completedColor);

		drawBox(pBuffer,
			vCurPos.x + completedBarSize,
			vCurPos.y,
			vCurPos.x + sCurSize.w,
			vCurPos.y + sCurSize.h,
			&remainingColor);
	}
}
