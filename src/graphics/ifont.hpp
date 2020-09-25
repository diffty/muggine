#ifndef IFONT_HPP
#define IFONT_HPP

#include "../core/common_types.hpp"


class IFont {
public:
	IFont() {};
	virtual ~IFont() {};

	virtual void draw(drawbuffer* pBuffer, int codepoint, int x, int y, int hSize, Color* color) = 0;

	virtual int getWidthForChar(int codepoint, int hSize = -1) = 0;
	virtual int getCharHeight() = 0;
};

#endif
