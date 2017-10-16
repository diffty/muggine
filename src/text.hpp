#ifndef TEXT_HPP
#define TEXT_HPP

#include "iwidget.hpp"
#include "font.hpp"


class Text :
	public IWidget
{
private:
	Font* m_pFont;
	char* m_szText;

public:
	Text(char* aText, Font* pFont, vect2df_t vPos);
	~Text();

	char* getText();
	void setText(char* szText);

	void draw(uint8* buffer);

	void drawChar(uint8* buffer, float x, float y, char c);
	void drawStr(uint8* buffer, float x, float y, char* szText);

	static char* intToStr(int iNum);
};

#endif