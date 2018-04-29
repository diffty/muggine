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
	Text(const char* szText, Font* pFont, vect2df_t vPos);
	Text(const char* szText, Font* pFont, float fXPos, float fYPos);
	Text(int iNum, Font* pFont, vect2df_t vPos);
	Text(int iNum, Font* pFont, float fXPos, float fYPos);
	~Text();
    
    void init(const char* szText, Font* pFont, float fXPos, float fYPos);
    void init(int iNum, Font* pFont, float fXPos, float fYPos);

	char* getText();
	void setText(const char* szText);
	void setText(int iNum, int padding = 0);

	void updateSize();

	void draw(uint8* buffer);

	void drawChar(uint8* buffer, float x, float y, char c);
	void drawStr(uint8* buffer, float x, float y, char* szText);

	static char* intToStr(int iNum, int padding = 0);
};

#endif
