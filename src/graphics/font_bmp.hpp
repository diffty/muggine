#ifndef FONT_BMP_HPP
#define FONT_BMP_HPP

#include "spritesheet.hpp"


class CSVReader;

class FontBitmap :
	public SpriteSheet
{
private:
	CSVReader* m_pCharSizesCsv;
	int* m_pCharSizesTable;
	int m_iSizeOffset;

public:
	FontBitmap(char* szFileName, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset);
	~FontBitmap();

	int getSizeForChar(char c);
};

#endif