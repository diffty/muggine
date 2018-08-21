#include "drawing.hpp"
#include "constants.hpp"


void drawLine(uint8* buffer, int fromX, int fromY, int toX, int toY, Color* color) {
#if TARGET_SDL || TARGET_SDL2
    float xSize = (float) (toX - fromX);
    float ySize = (float) (toY - fromY);

    float lineSize = ((xSize * xSize) + (ySize * ySize)) / 2.0;
    
    for (int i = 0; i < lineSize; i++) {
        int x = fromX + (toX-fromX) * ((float) i / lineSize);
        int y = fromY + (toY-fromY) * ((float) i / lineSize);
        
        *(buffer + (x + (y * SCREEN_WIDTH)) * SCREEN_BPP * sizeof(uint8)) = *color->get3DSFramebufferFormat();
        
        memcpy(
               buffer + (x + (y * SCREEN_WIDTH)) * SCREEN_BPP * sizeof(uint8),
               color->get3DSFramebufferFormat(),
               SCREEN_BPP * sizeof(uint8)
        );
    }
#endif
}

void drawBox(uint8* fb, int fromX, int fromY, int toX, int toY, Color* color) {
	int x, y;

#if TARGET_3DS
	int boxHeight = std::abs(toY - fromY);

	uint8* oneColorCol = new uint8[SCREEN_BPP * boxHeight];

	fromY = min(fromY, toY);
	toY = max(fromY, toY);

	for (y = 0; y < boxHeight; y++) {
		memmove(oneColorCol + (y * SCREEN_BPP * sizeof(uint8)),
			color->get3DSFramebufferFormat(),
			SCREEN_BPP * sizeof(uint8)
		);
	}

	for (x = fromX; x <= toX; x++) {
		memmove(fb + ((SCREEN_HEIGHT-1 - toY) * SCREEN_BPP * sizeof(uint8)) + (x * SCREEN_BPP * sizeof(uint8) * SCREEN_HEIGHT),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			SCREEN_BPP * boxHeight * sizeof(uint8)
		);
	}

#elif TARGET_SDL || TARGET_SDL2
	fromX = minInt(maxInt(minInt(fromX, toX), 0), SCREEN_WIDTH-1);
	toX = minInt(maxInt(maxInt(fromX, toX), 0), SCREEN_WIDTH-1);

	fromY = minInt(maxInt(fromY, 0), SCREEN_HEIGHT-1);
	toY = minInt(maxInt(toY, 0), SCREEN_HEIGHT-1);

	int boxWidth = fabs((float)(toX - fromX));

	uint8* oneColorCol = new uint8[SCREEN_BPP * boxWidth];

	for (x = 0; x < boxWidth; x++) {
		memmove(oneColorCol + (x * SCREEN_BPP * sizeof(uint8)),
			color->get3DSFramebufferFormat(),
			SCREEN_BPP * sizeof(uint8)
		);
	}

	for (y = fromY; y <= toY; y++) {
		memmove(fb + (fromX * SCREEN_BPP * sizeof(uint8)) + (y * SCREEN_BPP * sizeof(uint8) * SCREEN_WIDTH),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			SCREEN_BPP * boxWidth * sizeof(uint8)
		);
	}

#endif

	delete[] oneColorCol;
}

void rescaleBuffer(uint8* srcBuf, int srcW, int srcH, int srcBpp, uint8* dstBuf, int dstW, int dstH, int dstBpp) {
	int srcBufSize = srcW * srcH;
	int dstBufSize = dstW * dstH;
	int scaleRatio = dstBufSize / srcBufSize;

	for (int i = srcBufSize - 1; i >= 0; i--) {
		uint8* dstBufPtr = (dstBuf + (uint8) (i * srcBpp * scaleRatio));
		uint8* srcBufPtr = (srcBuf + (uint8) (i * srcBpp));
		// uint8 currVal = *(dstBufPtr + i);
		memcpy(dstBufPtr, srcBufPtr, srcBpp);
	}
}
