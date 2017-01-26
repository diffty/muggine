/*
	Hello World example made by Aurelio Mannara for ctrulib
	This code was modified for the last time on: 12/13/2014 01:00 UTC+1

	This wouldn't be possible without the amazing work done by:
	-Smealum
	-fincs
	-WinterMute
	-yellows8
	-plutoo
	-mtheall
	-Many others who worked on 3DS and I'm surely forgetting about
*/

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a >= b) ? a : b;
}

void drawBox(u8* fb, int fromX, int fromY, int toX, int toY, u8* color) {
	int x, y;
	int boxHeight = abs(toY - fromY);

	fromY = min(fromY, toY);
	toY = max(fromY, toY);

	u8* oneColorCol = (u8*) malloc(3 * boxHeight * sizeof(u8));

	for (y = 0; y < boxHeight; y++) {
		memmove(oneColorCol + (y * 3 * sizeof(u8)),
			color,
			3 * sizeof(u8)
		);
	}

	for (x = fromX; x <= toX; x++) {
		memmove(fb + ((239 - toY) * 3 * sizeof(u8)) + (x * 3 * sizeof(u8) * 240),  // Attention, on inverse from et to et 239- parce que la DS a un système de coord qui part d'en bas à gauche
			oneColorCol,
			3 * boxHeight * sizeof(u8)
		);
	}

	free(oneColorCol);
}


typedef struct SamplerGridCell {
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
} SamplerGridCell;

typedef struct SamplerGrid {
	unsigned int h;
	unsigned int w;
	unsigned int x;
	unsigned int y;
	unsigned int nbCellX;
	unsigned int nbCellY;
	unsigned int nbCells;
	unsigned char* cellState;
	unsigned int* pressedCells;
	unsigned int nbPressedCells;
	unsigned int* pressedCellsCurrentLoop;
	unsigned int nbPressedCellsCurrentLoop;
} SamplerGrid;

typedef struct Vector2 {
	long x;
	long y;
} Vector2;


void initSamplerGrid(SamplerGrid* samplerGrid) {
	int i;
	int nbCells;

	samplerGrid->w = 300;
	samplerGrid->h = 200;
	samplerGrid->nbCellX = 10;
	samplerGrid->nbCellY = 8;
	samplerGrid->x = 0;
	samplerGrid->y = 0;
	samplerGrid->nbCells = samplerGrid->nbCellX * samplerGrid->nbCellY;
	samplerGrid->cellState = (unsigned char *) malloc(samplerGrid->nbCells);
	samplerGrid->nbPressedCells = 0;
	samplerGrid->pressedCells = (unsigned char *) malloc(samplerGrid->nbCells);
	samplerGrid->pressedCellsCurrentLoop = (unsigned char *) malloc(samplerGrid->nbCells);
	
	for (i = 0; i < samplerGrid->nbCells; i++) {
		samplerGrid->cellState[i] = 0;
	}
}

void drawSamplerGrid(u8* fb, const SamplerGrid samplerGrid) {
	int cellW = samplerGrid.w / samplerGrid.nbCellX;
	int cellH = samplerGrid.h / samplerGrid.nbCellY;

	u8* colorOff = (u8*) malloc(3 * sizeof(u8));
	
	colorOff[0] = 0xFF;
	colorOff[1] = 0x2F;
	colorOff[2] = 0xAF;

	u8* colorOn = (u8*) malloc(3 * sizeof(u8));
	
	colorOn[0] = 0xAF;
	colorOn[1] = 0x2F;
	colorOn[2] = 0xFF;

	u8* colorPressed = (u8*) malloc(3 * sizeof(u8));
	
	colorPressed[0] = 0x2F;
	colorPressed[1] = 0xAF;
	colorPressed[2] = 0xFF;

	for (int y = 0; y < samplerGrid.nbCellY; y++) {
		for (int x = 0; x < samplerGrid.nbCellX; x++) {
			if (samplerGrid.cellState[y * samplerGrid.nbCellX + x] == 1) {
				drawBox(fb,
					samplerGrid.x + cellW * x,
					samplerGrid.y + cellH * y,
					samplerGrid.x + cellW * (x + 1) - 2,
					samplerGrid.y + cellH * (y + 1) - 1,
					colorOn
				);
			}
			else if (samplerGrid.cellState[y * samplerGrid.nbCellX + x] == 2 || samplerGrid.cellState[y * samplerGrid.nbCellX + x] == 4) {
				drawBox(fb,
					samplerGrid.x + cellW * x,
					samplerGrid.y + cellH * y,
					samplerGrid.x + cellW * (x + 1) - 2,
					samplerGrid.y + cellH * (y + 1) - 1,
					colorPressed
				);
			}
			else {
				drawBox(fb,
					samplerGrid.x + cellW * x,
					samplerGrid.y + cellH * y,
					samplerGrid.x + cellW * (x + 1) - 2,
					samplerGrid.y + cellH * (y + 1) - 1,
					colorOff
				);
			}
		}
	}

	free(colorOff);
	free(colorOn);
}

void freeSamplerGrid(SamplerGrid* samplerGrid) {
	free(samplerGrid->cellState);
}

void releaseSamplerGridCell(SamplerGrid* samplerGrid, int cellId) {
	int i;

	for (i = 0; i < samplerGrid->nbPressedCells; i++) {
		if (samplerGrid->pressedCells[i] == cellId) break;
	}

	for (i = i+1; i < samplerGrid->nbPressedCells; i++) {
		samplerGrid->pressedCells[i-1] = samplerGrid->pressedCells[i];
	}

	samplerGrid->pressedCells[i-1] = -1;
	samplerGrid->nbPressedCells--;
}

void cellPressedInSamplerGrid(SamplerGrid* samplerGrid, Vector2 gridCell) {
	int i;
	unsigned int cellId = gridCell.x + gridCell.y * samplerGrid->nbCellX;

	if (samplerGrid->cellState[cellId] == 2 || samplerGrid->cellState[cellId] == 4) {
		return;
	}

	samplerGrid->cellState[cellId] = (samplerGrid->cellState[cellId] == 0) ? 2 : 4;
	samplerGrid->pressedCells[samplerGrid->nbPressedCells] = cellId;
	samplerGrid->nbPressedCells++;
}

char isCellPressed(SamplerGrid* samplerGrid, Vector2 gridCell) {
	int i;

	unsigned int cellId = gridCell.x + gridCell.y * samplerGrid->nbCellX;

	for (i = 0; i < samplerGrid->nbPressedCells; i++) {
		if (samplerGrid->pressedCells[i] == cellId) {
			return 1;
		}
	}

	return 0;
}

unsigned char isPointInSamplerGrid(SamplerGrid* samplerGrid, Vector2 pt) {
	if ((samplerGrid->x <= pt.x && samplerGrid->x + samplerGrid->w >= pt.x)
		|| (samplerGrid->y <= pt.y && samplerGrid->y + samplerGrid->h >= pt.y))
		return 1;
	else
		return 0;
}

unsigned char getCellInSamplerGridFromPoint(SamplerGrid* samplerGrid, Vector2 pt, Vector2* gridCell) {
	gridCell->x = (int) (((float) (pt.x - samplerGrid->x) / samplerGrid->w) * samplerGrid->nbCellX);
	gridCell->y = (int) (((float) (pt.y - samplerGrid->y) / samplerGrid->h) * samplerGrid->nbCellY);

	if (gridCell->x < 0 || gridCell->y < 0 || gridCell->x >= samplerGrid->nbCellX || gridCell->y >= samplerGrid->nbCellY)
		return 0;
	else
		return 1;
}

int main(int argc, char **argv)
{
	int boxX = 50;
	int boxY = 50;
	int boxW = 50;
	int boxH = 50;
	int boxVelX = 1;
	int boxVelY = 1;

	unsigned char isCellStillPressed = 0;

	int i, j;
	unsigned int cellId;

	SamplerGrid samplerGrid;

	initSamplerGrid(&samplerGrid);

	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);

	printf("\x1b[20;15HPress Start to exit.");

	//We don't need double buffering in this example. In this way we can draw our image only once on screen.
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	int colorDir = 1; 

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		//Read the touch screen coordinates
		touchPosition touch;
		hidTouchRead(&touch);

		samplerGrid.nbPressedCellsCurrentLoop = 0;

		if (touch.px + touch.py > 0) {
			Vector2 touchPt;
			touchPt.x = touch.px;
			touchPt.y = touch.py;

			Vector2 inGridTouchPt;
			if (getCellInSamplerGridFromPoint(&samplerGrid, touchPt, &inGridTouchPt)) {
				cellId = inGridTouchPt.x + inGridTouchPt.y * samplerGrid.nbCellX;

				if (!isCellPressed(&samplerGrid, inGridTouchPt)) {
					printf("cell %d pressed\n", cellId);
					cellPressedInSamplerGrid(&samplerGrid, inGridTouchPt);
				}

				samplerGrid.pressedCellsCurrentLoop[samplerGrid.nbPressedCellsCurrentLoop] = cellId;
				samplerGrid.nbPressedCellsCurrentLoop++;
			}
		}

		for (i = samplerGrid.nbPressedCells - 1; i >= 0; i--) {
			isCellStillPressed = 0;
			
			for (j = 0; j < samplerGrid.nbPressedCellsCurrentLoop; j++) {
				if (samplerGrid.pressedCells[i] == samplerGrid.pressedCellsCurrentLoop[j]) {
					isCellStillPressed = 1;
				}
			}

			if (isCellStillPressed == 0) {
				cellId = samplerGrid.pressedCells[i];
				printf("cell %d released\n", cellId);
				if (samplerGrid.cellState[cellId] == 2) {
					samplerGrid.cellState[cellId] = 1;
				}
				else if (samplerGrid.cellState[cellId] == 4) {
					samplerGrid.cellState[cellId] = 0;
				}

				releaseSamplerGridCell(&samplerGrid, cellId);
			}
		}

		drawSamplerGrid(fb, samplerGrid);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();

	}

	freeSamplerGrid(&samplerGrid);

	// Exit services
	gfxExit();
	return 0;
}
