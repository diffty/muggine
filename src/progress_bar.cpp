#include "progress_bar.hpp"



ProgressBar::ProgressBar(vect2df_t vPos, size2df_t sSize, float fMinValue, float fMaxValue, float fCurValue) :
	IWidget(vPos.x, vPos.y, sSize.w, sSize.h)
{
	m_fMinValue = fMinValue;
	m_fMaxValue = fMaxValue;
	m_fCurValue = fCurValue;
}


ProgressBar::~ProgressBar() {
    
}

void ProgressBar::setValue(float fCurValue) {
	m_fCurValue = fCurValue;
}

void ProgressBar::setMinValue(float fMinValue) {
	m_fMinValue = fMinValue;
}

void ProgressBar::setMaxValue(float fMaxValue) {
	m_fMaxValue = fMaxValue;
}

void ProgressBar::setBorderColor(unsigned int r, unsigned int g, unsigned int b) {
    m_borderColor.setColor(r, g, b);
}

void ProgressBar::setRemainingColor(unsigned int r, unsigned int g, unsigned int b) {
    m_remainingColor.setColor(r, g, b);
}

void ProgressBar::setCompletedColor(unsigned int r, unsigned int g, unsigned int b) {
    m_completedColor.setColor(r, g, b);
}

void ProgressBar::draw(uint8* fb) {
	if (m_bIsActive) {
		drawChildren(fb);
        
		vect2df_t vCurPos = m_rect.getPos();
		size2df_t sCurSize = m_rect.getSize();

		float completedBarSize = (-m_fMinValue + m_fCurValue) / (m_fMaxValue - m_fMinValue) * (sCurSize.w-1);

		drawBox(fb,
			vCurPos.x + 1,
			vCurPos.y + 1,
			vCurPos.x + completedBarSize,
			vCurPos.y + sCurSize.h - 1 - 1,
			&m_completedColor);

		drawBox(fb,
			vCurPos.x + completedBarSize,
			vCurPos.y + 1,
			vCurPos.x + sCurSize.w - 1 - 1,
			vCurPos.y + sCurSize.h - 1,
			&m_remainingColor);
        
        drawLine(fb,
                 vCurPos.x + 1,
                 vCurPos.y,
                 vCurPos.x + 1 + sCurSize.w - 1 - 2,
                 vCurPos.y,
                 &m_borderColor);
        
        drawLine(fb,
                 vCurPos.x + 1,
                 vCurPos.y + sCurSize.h - 1,
                 vCurPos.x + 1 + sCurSize.w - 1 - 2,
                 vCurPos.y + sCurSize.h - 1,
                 &m_borderColor);
        
        drawLine(fb,
                 vCurPos.x,
                 vCurPos.y + 1,
                 vCurPos.x,
                 vCurPos.y + 1 + sCurSize.h - 2,
                 &m_borderColor);
        
        drawLine(fb,
                 vCurPos.x + sCurSize.w - 2,
                 vCurPos.y + 1,
                 vCurPos.x + sCurSize.w - 2,
                 vCurPos.y + 1 + sCurSize.h - 2,
                 &m_borderColor);
	}
}
