#ifndef TINY_PROGRESS_BAR_HPP
#define TINY_PROGRESS_BAR_HPP

#include "../../graphics/sprite.hpp"
#include "../../graphics/drawing.hpp"


class TinyProgressBar : public IWidget
{
private:
	float m_fCurValue;
	float m_fMinValue;
	float m_fMaxValue;

public:
	TinyProgressBar(vect2df_t vPos, size2df_t sSize, float fMinValue = 0., float fMaxValue = 1., float fCurValue = 0.);
	~TinyProgressBar();

	void setValue(float fValue);
	void setMinValue(float fMinValue);
	void setMaxValue(float fMaxValue);

	void draw(uint8* fb);
};



#endif