#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include "../../graphics/sprite.hpp"
#include "../../graphics/drawing.hpp"


class ProgressBar : public IWidget
{
private:
	float m_fCurValue;
	float m_fMinValue;
	float m_fMaxValue;
    
    Color m_borderColor;
    Color m_remainingColor;
    Color m_completedColor;

public:
	ProgressBar(vect2df_t vPos, size2df_t sSize, float fMinValue = 0., float fMaxValue = 1., float fCurValue = 0.);
	ProgressBar(float fXPos, float fYPos, float fWSize, float fHSize, float fMinValue = 0., float fMaxValue = 1., float fCurValue = 0.);
	~ProgressBar();
    
    void init(float fXPos, float fYPos, float fWSize, float fHSize, float fMinValue, float fMaxValue, float fCurValue);

	void setValue(float fValue);
	void setMinValue(float fMinValue);
	void setMaxValue(float fMaxValue);
    
    void setBorderColor(unsigned int r, unsigned int g, unsigned int b);
    void setRemainingColor(unsigned int r, unsigned int g, unsigned int b);
    void setCompletedColor(unsigned int r, unsigned int g, unsigned int b);

	void draw(uint8* fb);
};



#endif
