#ifndef FONT_HPP
#define FONT_HPP

#include "spritesheet.hpp"

class CSVReader;

class Font :
	public SpriteSheet
{
private:
	CSVReader* m_pCharSizesCsv;
	int* m_pCharSizesTable;
	int m_iSizeOffset;

public:
	Font(char* szFileName, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset);
	~Font();

	int getSizeForChar(char c);
};

#endif