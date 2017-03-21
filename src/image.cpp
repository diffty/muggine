#include "image.hpp"


Image::Image(char* fileName) {
	loadFromFile(fileName);
}

Image::~Image() {

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
	printf("Not enough memory for new image allocation.\n");
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

	printf("Size : %ld\n", *dataSize);
	printf("Start offset : %d\n", *startOffset);
	printf("Header size : %d\n", *headerSize);
	printf("Image width : %d\n", m_size.w);
	printf("Image height : %d\n", m_size.h);
	printf("Palette size : %d\n", m_paletteSize);

	nbPixels = (long) m_size.h * (long) m_size.w;

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
	m_pRawImgData = (byte *) malloc(nbPixels);
	m_pImgData = (uint8 *)malloc(nbPixels * SCREEN_BPP * sizeof(uint8));

	seekPtr = *startOffset;

	printf("NbPixels: %d\n", nbPixels);

	int imgDataPtr = 0;

	while (seekPtr < (nbPixels + (*startOffset))) {
		nBytesToRead = min((nbPixels - (seekPtr - (*startOffset))), FREAD_BUFFER_SIZE);

		fseek(fp, seekPtr, SEEK_SET);
		fread(fileBuf, 1, nBytesToRead, fp);

		for (int i = 0; i < nBytesToRead; i++) {
			byte currByte = fileBuf[i];
			m_pImgData[(imgDataPtr + i) * SCREEN_BPP]     = m_aPalette[(int)currByte].b;
			m_pImgData[(imgDataPtr + i) * SCREEN_BPP + 1] = m_aPalette[(int)currByte].g;
			m_pImgData[(imgDataPtr + i) * SCREEN_BPP + 2] = m_aPalette[(int)currByte].r;
#if TARGET_WIN
			m_pImgData[(imgDataPtr + i) * SCREEN_BPP + 3] = 0;
#endif
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
		for (int i = 0; i < m_size.w; i++) {
			memcpy(buffer + (y * SCREEN_BPP) + ((x + i) * SCREEN_HEIGHT * SCREEN_BPP),
				m_pImgData + (((m_size.w - i - 1)) * (m_size.h * SCREEN_BPP)),
				m_size.h * SCREEN_BPP);
		}
#elif TARGET_WIN
		for (int i = 0; i < m_size.h; i++) {
			memcpy(buffer + (x * SCREEN_BPP) + ((y + i) * SCREEN_WIDTH * SCREEN_BPP),
				m_pImgData + (((m_size.h - i - 1)) * (m_size.w * SCREEN_BPP)),
				m_size.w * SCREEN_BPP);
		}
#endif
	}
}
