#ifndef FONT_BMP_HPP
#define FONT_BMP_HPP

#include "spritesheet.hpp"

#include "ifont.hpp"


class CSVReader;

class FontBitmap : public IFont
{
private:
	CSVReader* m_pCharSizesCsv;
	int* m_pCharSizesTable;
	int m_iSizeOffset;
	SpriteSheet m_sprSht;

public:
	FontBitmap(char* szFileName, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset);
	~FontBitmap();

	void draw(uint8* pBuffer, char c, int x, int y, int size, Color* color);

	int getWidthForChar(char c, int hSize = -1);
	int getCharHeight();

	size2d_t getCharSize();
};

#endif