#ifndef FONT_TTF_HPP
#define FONT_TTF_HPP

#include <cstdio>

#include "ifont.hpp"

#include "../libs/stb_truetype.h"
#include "../core/common_types.hpp"
#include "../core/constants.hpp"


class FontTTF : public IFont
{
public:
	FontTTF(char* szFileName);
	~FontTTF();

	void draw(uint8* pBuffer, char c, int x, int y, int hSize, Color* color);

	int getWidthForChar(char c, int hSize = -1);
	int getCharHeight();

private:
	stbtt_fontinfo m_font;
	unsigned char* m_pFontFileBuffer;
};

#endif
