#ifndef TEXT_HPP
#define TEXT_HPP

#include "iwidget.hpp"
#include "font.hpp"


class Text :
	public IWidget
{
private:
	Font* m_pFont;
	char* m_aText;

public:
	Text(char* aText, Font* pFont, vect2df_t vPos);
	~Text();

	void draw(uint8* buffer);

	void drawChar(uint8* buffer, float x, float y, char c);
	void drawStr(uint8* buffer, float x, float y, char* text);

	static char** intToStr(int iNum);
};

#endif