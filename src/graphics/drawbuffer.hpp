#ifndef DRAWBUFFER_HPP
#define DRAWBUFFER_HPP


#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>

#include "../core/constants.hpp"
#include "../core/common_types.hpp"
#include "../utils/math_tools.hpp"
#include "../utils/linked_list.hpp"

#define FREAD_BUFFER_SIZE   1000



class DrawBuffer {
protected:
	uint8* m_pImgData;
    drawbuffer m_drawBuffer;
	size2df_t m_size;
	long* m_mask;
	long* m_nbZoneByLine;
	long** m_maskIdByLine;
	uint m_maskNbZone;
	int m_paletteSize;
	color_t* m_aPalette;
    bool m_bIsMasked;
    
public:
	DrawBuffer();
	~DrawBuffer();
    
    void createBuffer(int w, int h /*, byte* paletteData = NULL */);
    void wipeBuffer();

    void draw(drawbuffer* pBuffer, int dstX, int dstY, bool reversed, bool masked);
    void draw(drawbuffer* pBuffer, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, bool reversed, bool masked);
    
    void detectTransparency(color_t transpClr);
    void clear();
    void clear(Color clr);
    drawbuffer* getBuffer();

	size2df_t getSize() { return m_size; }
	size2d_t getSizei() {
		size2d_t newSize;
		newSize.w = m_size.w;
		newSize.h = m_size.h;
		return newSize;
	}
};

#endif
