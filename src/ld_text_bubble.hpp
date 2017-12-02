#ifndef LD_TEXT_BUBBLE_HPP
#define LD_TEXT_BUBBLE_HPP

#include "iwidget.hpp"
#include "text.hpp"
#include "font.hpp"
#include "drawing.hpp"


class TextBubble : public IWidget
{
private:
	char* m_szText;
	Text* m_pTextWidget;
	Color m_cBorderColor;
	Color m_cFillColor;

public:
	TextBubble(char* szText, Font* pFont, float x, float y, float w, float h);
	~TextBubble();

	char* getText();
	void setText(char* szText);

	void translate(int x, int y, ETransformMode transformMode = TRANSFORM_REL);

	void updateSize();

	void draw(uint8* buffer);
};


#endif