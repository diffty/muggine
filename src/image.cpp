#include "image.hpp"


Image::Image(char* fileName) {
	loadFromFile(fileName);
}

Image::~Image() {
	free(m_pImgData);
	free(m_aPalette);
}


void Image::loadFromFile(char* fileName) {
	FILE* fp;
	int fh;
	int nBytes, nBytesToRead;
	int i, j, k;

	int* imgW = (int *) malloc(4);
	int* imgH = (int *) malloc(4);
	int* imgPalSize = (int *) malloc(4);

	unsigned long* dataSize = (unsigned long *) malloc(4);
	int* startOffset = (int *) malloc(4);
	int* headerSize  = (int *) malloc(4);
	unsigned long nbPixels;

	byte* fileBuf = (byte *) malloc(FREAD_BUFFER_SIZE);
	long seekPtr;

	byte* paletteData;

	printf("Loading image %s.\n", fileName);

	/*if ((image = (image_t *) malloc(sizeof(image_t))) == NULL) {
	printf("Not enough memory forS new image allocation.\n");
	exit(1);
	}*/
    
	if ((fp = fopen(fileName, "rb")) == NULL) {
		printf("Can't read image file %s. Aborting.\n", fileName);
		exit(1);
	}
    
	// Reading header
	fseek(fp, 0x0002, SEEK_SET);
	fread(dataSize, 4, 1, fp);
	fseek(fp, 0x000A, SEEK_SET);
	fread(startOffset, 4, 1, fp);
	fseek(fp, 0x000E, SEEK_SET);
	fread(headerSize, 4, 1, fp);
	fseek(fp, 0x0012, SEEK_SET);
	fread(imgW, 4, 1, fp);
	fseek(fp, 0x0016, SEEK_SET);
	fread(imgH, 4, 1, fp);
	fseek(fp, 0x002E, SEEK_SET);
	fread(imgPalSize, 4, 1, fp);

	m_size.w = *imgW;
	m_size.h = *imgH;
	m_paletteSize = *imgPalSize;

	if (m_paletteSize == 0) m_paletteSize = 256;

	nbPixels = (long) m_size.h * (long) m_size.w;

	long rowPadding = 4 - m_size.w % 4;

	printf("Size : %ld\n", *dataSize);
	printf("Start offset : %d\n", *startOffset);
	printf("Header size : %d\n", *headerSize);
	printf("Image width : %d\n", m_size.w);
	printf("Image height : %d\n", m_size.h);
	printf("Palette size : %d\n", m_paletteSize);
	printf("Nb pixels : %d\n", nbPixels);
	printf("Padding: %d\n", rowPadding);

	// Reading palette
	paletteData = (byte *) malloc(m_paletteSize*4);

	fseek(fp, (*headerSize)+14, SEEK_SET);
	fread(paletteData, 1, m_paletteSize*4, fp);

	m_aPalette = (color_t *) malloc(sizeof(color_t) * m_paletteSize);

	for (i = 0; i < m_paletteSize; i++) {
		m_aPalette[i].b = paletteData[i*4];
		m_aPalette[i].g = paletteData[i*4+1];
		m_aPalette[i].r = paletteData[i*4+2];
	}

	// Reading pixel array
	m_pImgData = (uint8 *) malloc(nbPixels * SCREEN_BPP * sizeof(uint8));

	seekPtr = *startOffset;

	int imgDataPtr = 0;

	int nbPixelsWPadding = nbPixels + (m_size.h - 1) * rowPadding;

	while (seekPtr < (nbPixelsWPadding + (*startOffset))) {
		nBytesToRead = min((nbPixelsWPadding - (seekPtr - (*startOffset))), FREAD_BUFFER_SIZE);
        
        
		fseek(fp, seekPtr, SEEK_SET);
		fread(fileBuf, 1, nBytesToRead, fp);

		int currPixNb = 0;

		for (int i = 0; i < nBytesToRead; i++) {
			if (currPixNb % m_size.w == 0 && currPixNb > 0) {
				i += rowPadding;
			}

#ifdef TARGET_3DS
            int fileBufSeek = ((currPixNb % m_size.h) * (m_size.w + rowPadding)) + (currPixNb / m_size.h);
#else
			int fileBufSeek = i;
#endif
            
			byte currByte = fileBuf[fileBufSeek];

			m_pImgData[imgDataPtr + (currPixNb * SCREEN_BPP)] = m_aPalette[(int)currByte].b;
			m_pImgData[imgDataPtr + (currPixNb * SCREEN_BPP) + 1] = m_aPalette[(int)currByte].g;
			m_pImgData[imgDataPtr + (currPixNb * SCREEN_BPP) + 2] = m_aPalette[(int)currByte].r;
#if TARGET_SDL
			m_pImgData[imgDataPtr + (currPixNb * SCREEN_BPP) + 3] = 0;
#endif
			currPixNb++;
		}

		seekPtr += nBytesToRead;
		imgDataPtr += nBytesToRead;
	}


	fclose(fp);

	/*
	m_mask = (u8 *) malloc(2 * sizeof(u8));
	
	// Building transparency info
	j = 0;
	k = 0;
	m_maskNbZone = 0;

	for (i = 0; i < nbPixels; i++) {
		if (m_pImgData[i] != m_pImgData[0]) {
			j++;
		}

		if (i < (nbPixels) - 1) {
			if (m_pImgData[i] != m_pImgData[i+1]) {
				if (m_pImgData[i+1] == m_pImgData[0] || i+1 == (nbPixels) - 1 || (i > 0 && i % m_size.w == 0)) {
					m_mask = (u8 *) realloc(m_mask, 2 * (m_maskNbZone + 1) * sizeof(u8));

					m_mask[k] = i-j+1;
					m_mask[k+1] = j;

					k += 2;
					j = 0;

					m_maskNbZone++;
				}
			}
		}
	}
	*/

	free(dataSize);
	free(startOffset);
	free(headerSize);
	free(fileBuf);
	free(paletteData);
	free(imgW);
	free(imgH);
	free(imgPalSize);
}

void Image::draw(uint8* buffer, int x, int y, bool reversed, bool masked) {
    int xb, yb;
    unsigned int imgBufIdx, zoneSize;
	unsigned int overflowLeft = 0, overflowRight = 0, overflowTop = 0, overflowBottom = 0;

	overflowLeft = max(0, -x);
	overflowRight = max(0, (x + m_size.w) - SCREEN_WIDTH);
	overflowTop = max(0, -y);
	overflowBottom = max(0, (y + m_size.h) - SCREEN_HEIGHT);

	// printf("%d, %d, %d, %d\n", overflowLeft, overflowRight, overflowBottom, overflowTop);

	if (overflowLeft > m_size.w || overflowRight > m_size.w || overflowTop > m_size.h || overflowBottom > m_size.h) {
		return;
	}

  	if (masked) {
		for (int i = 0; i < m_maskNbZone; i++) {
			imgBufIdx = m_mask[i*2];
			zoneSize = m_mask[i*2+1];

			xb = imgBufIdx % m_size.w;

			if (reversed)
				yb = imgBufIdx / m_size.w;
			else
				yb = m_size.h - (imgBufIdx / m_size.w) - 1;

			// Clipping
			if (x + xb + zoneSize < 0 || x + xb >= SCREEN_WIDTH || y + yb < 0 || y + yb >= SCREEN_HEIGHT) {
				continue;
			}
			else if (x + xb + zoneSize > SCREEN_WIDTH) {
				zoneSize = zoneSize - ((x + xb + zoneSize) % SCREEN_WIDTH);
			}
			else if (x + xb < 0) {
				imgBufIdx += -(x + xb);
				zoneSize = zoneSize + (x + xb);
				xb = imgBufIdx % m_size.w;
			}

			// ...Then current line copy
			/*memcpy(buffer + (x + xb) + ((y * SCREEN_WIDTH) + (yb * SCREEN_WIDTH)),
			       m_pImgData + imgBufIdx,
			       zoneSize);*/

			// ...Then current line copy
			memcpy(buffer + ((y + yb) * SCREEN_BPP) + ((x * SCREEN_HEIGHT * SCREEN_BPP) + (xb * SCREEN_HEIGHT)),
			       m_pImgData + imgBufIdx,
			       zoneSize);
		}
	}
	else {
#if TARGET_3DS
		for (int i = overflowLeft; i < m_size.w - overflowRight; i++) {
			memcpy(buffer + ((SCREEN_HEIGHT - m_size.h - 1 - y) * SCREEN_BPP) + ((x + i) * SCREEN_HEIGHT * SCREEN_BPP),
				m_pImgData + (i * m_size.h * SCREEN_BPP),
				(m_size.h - overflowTop - overflowBottom) * SCREEN_BPP);
		}
#elif TARGET_SDL
		for (int i = overflowTop; i < m_size.h - overflowBottom; i++) {
			memcpy(buffer + (max(0, x) * SCREEN_BPP) + ((max(0, y) + i - overflowTop) * SCREEN_WIDTH * SCREEN_BPP),
				m_pImgData + ((m_size.h - i - 1) * (m_size.w * SCREEN_BPP)) + (overflowLeft * SCREEN_BPP),
				(m_size.w - overflowLeft - overflowRight) * SCREEN_BPP);
		}
#endif
	}
}
