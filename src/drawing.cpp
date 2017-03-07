#include "drawing.hpp"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#ifdef TARGET_3DS
#define SCREEN_BPP		3
#elif TARGET_WIN
#define SCREEN_BPP		4
#endif



void drawBox(uint8* fb, int fromX, int fromY, int toX, int toY, Color* color) {
	int x, y;

#ifdef TARGET_3DS
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

#elif TARGET_WIN
	int boxWidth = fabs(toX - fromX);

	uint8* oneColorCol = new uint8[SCREEN_BPP * boxWidth];

	fromX = min(fromX, toX);
	toX = max(fromX, toX);

	for (x = 0; x < boxWidth; x++) {
		memmove(oneColorCol + (x * SCREEN_BPP * sizeof(uint8)),
			color->get3DSFramebufferFormat(),
			SCREEN_BPP * sizeof(uint8)
		);
	}

	for (y = fromY; y <= toY; y++) {
		memmove(fb + ((SCREEN_WIDTH - 1 - toX) * SCREEN_BPP * sizeof(uint8)) + (y * SCREEN_BPP * sizeof(uint8) * SCREEN_WIDTH),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			SCREEN_BPP * boxWidth * sizeof(uint8)
		);
	}

#endif

	delete oneColorCol;
}
