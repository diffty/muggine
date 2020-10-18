#include "image.hpp"

 
Image::Image(const char* fileName) {
	loadFromFile(fileName);
    Color bgClr(0, 255, 0);
    detectTransparency(bgClr.getColor());
}

Image::~Image() {
	free(m_aPalette);
}

void Image::loadFromFile(const char* fileName) {
	FILE* fp;
	int nBytesToRead;
	int i, j;

	int* imgW = (int *) malloc(4);
	int* imgH = (int *) malloc(4);
	int* imgPalSize = (int *) malloc(4);

	unsigned long* dataSize = (unsigned long *) malloc(4);
	int* startOffset = (int *) malloc(4);
	int* headerSize  = (int *) malloc(4);
	unsigned long nbPixels;
    
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

    long rowPadding = 0;
    
    if ((int) m_size.w % 4 != 0)
		rowPadding = 4 - (int) m_size.w % 4;

	printf("Size : %ld\n", *dataSize);
	printf("Start offset : %d\n", *startOffset);
	printf("Header size : %d\n", *headerSize);
	printf("Image width : %ld\n", m_size.w);
	printf("Image height : %ld\n", m_size.h);
	printf("Palette size : %d\n", m_paletteSize);
	printf("Nb pixels : %ld\n", nbPixels);
	printf("Padding: %ld\n", rowPadding);
    
    // Un buffer trop gros peut être problématique sur une ptite machine
    // int bufferSize = (int) (*imgW + rowPadding) * 3 * 10;
    int bufferSize = (int) (*imgW + rowPadding) * 3 * *imgH;
    
    // Allocating read buffer
    byte* fileBuf = (byte *) malloc(bufferSize);

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

	// Create image buffer
    createBuffer(m_size.w, m_size.h);

	seekPtr = *startOffset;

	long nbPixelsWPadding = nbPixels + (m_size.h - 1) * rowPadding;
    long imgWidthWPadding = m_size.w + rowPadding;
    
    long currPixNb = 0;
    
    long imgDataPtr = 0;
    
    bool inOpaque = false;
    
    LinkedList lOpaqueInfo;
    initList(&lOpaqueInfo);

#if TARGET_3DS
	m_nbZoneByLine = new long[(int) m_size.w];
	for (i = 0; i < m_size.w; i++)
		m_nbZoneByLine[i] = 0;
#else
    m_nbZoneByLine = new long[(int) m_size.h];
    for (i = 0; i < m_size.h; i++)
        m_nbZoneByLine[i] = 0;
#endif

	while (seekPtr < (nbPixelsWPadding + (*startOffset))) {
		nBytesToRead = minInt((int) (nbPixelsWPadding - (seekPtr - (long) (*startOffset))), bufferSize);
        
		fseek(fp, seekPtr, SEEK_SET);
		fread(fileBuf, 1, nBytesToRead, fp);

        long fileBufSeek = 0;

        int j = 0;

#if TARGET_3DS
        for (int i = 0; i < nBytesToRead - rowPadding * m_size.h; i++) {
#else
        for (int i = 0; i < nBytesToRead; i++) {
#endif
            
            if ((int) currPixNb % (int) m_size.w == 0 && currPixNb > 0) {
                i += rowPadding;
            }

#if TARGET_3DS
            /*fileBufSeek = ((currPixNb % m_size.w) * imgWidthWPadding) + (currPixNb / m_size.w);
            imgDataPtr = ((currPixNb % m_size.w) * m_size.h) + (currPixNb / m_size.w);*/
            
            fileBufSeek = (currPixNb % (int) m_size.h) * imgWidthWPadding + (currPixNb / m_size.h);
            imgDataPtr = currPixNb;
#else
			fileBufSeek = i;
            imgDataPtr = currPixNb;
#endif

            byte currByte = fileBuf[fileBufSeek];
            
			bool doEndNow = false;
            
            m_pImgData[(imgDataPtr * SCREEN_BPP)]     = m_aPalette[(int)currByte].b;
			m_pImgData[(imgDataPtr * SCREEN_BPP) + 1] = m_aPalette[(int)currByte].g;
			m_pImgData[(imgDataPtr * SCREEN_BPP) + 2] = m_aPalette[(int)currByte].r;
#if TARGET_SDL || TARGET_SDL2
			m_pImgData[(imgDataPtr * SCREEN_BPP) + 3] = 0;
#endif
			currPixNb++;
                
            j++;
		}
        
		seekPtr += nBytesToRead;
	}

	fclose(fp);
    
    free(dataSize);
	free(startOffset);
	free(headerSize);
	free(fileBuf);
	free(paletteData);
	free(imgW);
	free(imgH);
	free(imgPalSize);
}
