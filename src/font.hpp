#ifndef FONT_HPP
#define FONT_HPP

#include "spritesheet.hpp"


class Font :
	public SpriteSheet
{
public:
	Font(char* szFileName, int iGridWidth, int iGridHeight, uint uLength);
	~Font();
};

#endif