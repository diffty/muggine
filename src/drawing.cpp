#include "drawing.hpp"


void drawBox(uint8* fb, int fromX, int fromY, int toX, int toY, Color* color) {
	int x, y;
	int boxHeight = std::abs(toY - fromY);

	fromY = min(fromY, toY);
	toY = max(fromY, toY);

	uint8* oneColorCol = new uint8[3 * boxHeight];

	for (y = 0; y < boxHeight; y++) {
		memmove(oneColorCol + (y * 3 * sizeof(uint8)),
			color->get3DSFramebufferFormat(),
			3 * sizeof(uint8)
		);
	}

	for (x = fromX; x <= toX; x++) {
		memmove(fb + ((239 - toY) * 3 * sizeof(uint8)) + (x * 3 * sizeof(uint8) * 240),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			3 * boxHeight * sizeof(uint8)
		);
	}

	delete oneColorCol;
}