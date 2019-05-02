#ifndef TEXT_HPP
#define TEXT_HPP

#include "../../core/iwidget.hpp"
#include "../../graphics/font_bmp.hpp"


class Text :
	public IWidget
{
private:
	FontBitmap* m_pFont;
	char* m_szText;

public:
	Text(const char* szText, FontBitmap* pFont, vect2df_t vPos);
	Text(const char* szText, FontBitmap* pFont, float fXPos, float fYPos);
	Text(int iNum, FontBitmap* pFont, vect2df_t vPos);
	Text(int iNum, FontBitmap* pFont, float fXPos, float fYPos);
	~Text();
    
    void init(const char* szText, FontBitmap* pFont, float fXPos, float fYPos);
    void init(int iNum, FontBitmap* pFont, float fXPos, float fYPos);

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
