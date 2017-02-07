#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include "common_types.hpp"
#include "mathf.hpp"

#define FREAD_BUFFER_SIZE   1000

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240



class Image {
private:
	u8* m_pRawImgData;
	u8* m_pImgData;
	size2d_t m_size;
	u8* m_mask;
	uint m_maskNbZone;
	int m_paletteSize;
	color_t* m_aPalette;

public:
	Image(char* fileName);
	~Image();

	void loadFromFile(char* fileName);
	void draw(u8* buffer, int x, int y, bool reversed, bool masked);

	size2d_t getSize() { return m_size; }
};

#endif