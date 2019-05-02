#ifndef FONT_TTF_HPP
#define FONT_TTF_HPP

#include <cstdio>

#define STB_TRUETYPE_IMPLEMENTATION
#include "../libs/stb_truetype.h"

#include "../core/common_types.hpp"
#include "../core/constants.hpp"


class FontTTF
{
private:
	stbtt_fontinfo m_font;

public:
	FontTTF(char* szFileName);
	~FontTTF();

	void draw(uint8* fb, char c, int x, int y, int size);

	int getSizeForChar(char c);
};

#endif