#include "drawing.hpp"


void drawBox(u8* fb, int fromX, int fromY, int toX, int toY, Color* color) {
	int x, y;
	int boxHeight = std::abs(toY - fromY);

	fromY = min(fromY, toY);
	toY = max(fromY, toY);

	u8* oneColorCol = new u8[3 * boxHeight];

	for (y = 0; y < boxHeight; y++) {
		memmove(oneColorCol + (y * 3 * sizeof(u8)),
			color->get3DSFramebufferFormat(),
			3 * sizeof(u8)
		);
	}

	for (x = fromX; x <= toX; x++) {
		memmove(fb + ((239 - toY) * 3 * sizeof(u8)) + (x * 3 * sizeof(u8) * 240),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			3 * boxHeight * sizeof(u8)
		);
	}

	delete oneColorCol;
}