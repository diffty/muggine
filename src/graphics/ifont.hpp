#ifndef IFONT_HPP
#define IFONT_HPP

#include "../core/common_types.hpp"


class IFont {
public:
	IFont() {};
	virtual ~IFont() {};

	virtual void draw(uint8* pBuffer, char c, int x, int y, int hSize, Color* color) = 0;

	virtual int getWidthForChar(char c, int hSize = -1) = 0;
	virtual int getCharHeight() = 0;
};

#endif