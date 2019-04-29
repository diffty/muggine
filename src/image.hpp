#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>

#include "constants.hpp"
#include "common_types.hpp"
#include "mathf.hpp"
#include "linked_list.hpp"

#define FREAD_BUFFER_SIZE   1000



class Image {
protected:
	uint8* m_pImgData;
	size2df_t m_size;
	long* m_mask;
	long* m_nbZoneByLine;
	long** m_maskIdByLine;
	uint m_maskNbZone;
	int m_paletteSize;
	color_t* m_aPalette;

public:
	Image(char* fileName);
	~Image();

	void loadFromFile(char* fileName);
    void draw(uint8* buffer, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, bool reversed, bool masked);
    void draw(uint8* buffer, int dstX, int dstY, bool reversed, bool masked);

	size2df_t getSize() { return m_size; }
	size2d_t getSizei() {
		size2d_t newSize;
		newSize.w = m_size.w;
		newSize.h = m_size.h;
		return newSize;
	}
};

#endif
