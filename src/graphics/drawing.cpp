#include "drawing.hpp"

#include "../core/constants.hpp"
#include "../utils/math_tools.hpp"


void drawLine(drawbuffer* pBuffer, int fromX, int fromY, int toX, int toY, Color* color) {
#if TARGET_3DS
    float xSize = (float) (toX - fromX);
    float ySize = (float) (toY - fromY);
    
    float lineSize = ((xSize * xSize) + (ySize * ySize)) / 2.0;
    
    for (int i = 0; i < lineSize; i++) {
        int x = fromX + (toX-fromX) * ((float) i / lineSize);
        int y = fromY + (toY-fromY) * ((float) i / lineSize);
        
        memcpy(pBuffer->buffer + (x * (pBuffer->height) + (pBuffer->height - 1 - y)) * SCREEN_BPP * sizeof(uint8),
               color->get3DSFramebufferFormat(),
               SCREEN_BPP * sizeof(uint8)
               );
    }
#elif TARGET_SDL || TARGET_SDL2
    float xSize = (float) (toX - fromX);
    float ySize = (float) (toY - fromY);

    float lineSize = ((xSize * xSize) + (ySize * ySize)) / 2.0;
    
    for (int i = 0; i < lineSize; i++) {
        int x = fromX + (toX-fromX) * ((float) i / lineSize);
        int y = fromY + (toY-fromY) * ((float) i / lineSize);
        
        memcpy(
               pBuffer->buffer + (x + (y * pBuffer->width)) * SCREEN_BPP * sizeof(uint8),
               color->get3DSFramebufferFormat(),
               SCREEN_BPP * sizeof(uint8)
        );
    }
#endif
}

void drawBox(drawbuffer* pBuffer, int fromX, int fromY, int toX, int toY, Color* color) {
    int x, y;

#if TARGET_3DS
    int boxHeight = std::abs(toY - fromY);

    uint8* oneColorCol = new uint8[SCREEN_BPP * boxHeight];

    fromY = minInt(fromY, toY);
    toY = maxInt(fromY, toY);

    for (y = 0; y < boxHeight; y++) {
        memmove(oneColorCol + (y * SCREEN_BPP * sizeof(uint8)),
            color->get3DSFramebufferFormat(),
            SCREEN_BPP * sizeof(uint8)
        );
    }

    for (x = fromX; x <= toX; x++) {
        memmove(pBuffer->buffer + ((pBuffer->height-1 - toY) * SCREEN_BPP * sizeof(uint8)) + (x * SCREEN_BPP * sizeof(uint8) * pBuffer->height),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
            oneColorCol,
            SCREEN_BPP * boxHeight * sizeof(uint8)
        );
    }

#elif TARGET_SDL || TARGET_SDL2
    fromX = minInt(maxInt(minInt(fromX, toX), 0), pBuffer->width-1);
    toX = minInt(maxInt(maxInt(fromX, toX), 0), pBuffer->width-1);

    fromY = minInt(maxInt(fromY, 0), pBuffer->height-1);
    toY = minInt(maxInt(toY, 0), pBuffer->height-1);

    int boxWidth = fabs((float)(toX - fromX + 1));

    uint8* oneColorCol = new uint8[SCREEN_BPP * boxWidth];

    for (x = 0; x < boxWidth; x++) {
        memmove(oneColorCol + (x * SCREEN_BPP * sizeof(uint8)),
            color->get3DSFramebufferFormat(),
            SCREEN_BPP * sizeof(uint8)
        );
    }

    for (y = fromY; y <= toY; y++) {
        memmove(pBuffer->buffer + (fromX * SCREEN_BPP * sizeof(uint8)) + (y * SCREEN_BPP * sizeof(uint8) * pBuffer->width),
            oneColorCol,
            SCREEN_BPP * boxWidth * sizeof(uint8)
        );
    }

#endif

    delete[] oneColorCol;
}

void rescaleBuffer(drawbuffer* srcBuf, int srcW, int srcH, int srcBpp, drawbuffer* dstBuf, int dstW, int dstH, int dstBpp) {
	int srcBufSize = srcW * srcH;
	int dstBufSize = dstW * dstH;
	int scaleRatio = dstBufSize / srcBufSize;

	for (int i = srcBufSize - 1; i >= 0; i--) {
		uint8* dstBufPtr = (dstBuf->buffer + (uint8) (i * srcBpp * scaleRatio));
		uint8* srcBufPtr = (srcBuf->buffer + (uint8) (i * srcBpp));
		// uint8 currVal = *(dstBufPtr + i);
		memcpy(dstBufPtr, srcBufPtr, srcBpp);
	}
}
