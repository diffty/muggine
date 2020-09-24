#include "image.hpp"

 
Image::Image(char* fileName) {
	loadFromFile(fileName);
}

Image::~Image() {
	free(m_pImgData);
	free(m_aPalette);
    delete m_nbZoneByLine;
}

void Image::loadFromFile(char* fileName) {
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

	// Reading pixel array
	m_pImgData = (uint8 *) malloc(nbPixels * SCREEN_BPP * sizeof(uint8));

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

			// Transparency detection
			if (currPixNb == 0) { // If first pixel is opaque
				if ((int)currByte != 0) {
					inOpaque = true;
					LLNode* lnOpaqueInfo = new LLNode;
					lnOpaqueInfo->pData = new long;
					*((long*)lnOpaqueInfo->pData) = currPixNb;
					addNodeToList(&lOpaqueInfo, lnOpaqueInfo);
				}
			}
			else {
#if TARGET_3DS
				if ((inOpaque && (currByte == 0 || ((int) currPixNb + 1) % (int) m_size.h == 0)) || (!inOpaque && currByte != 0)) {
#else
                if ((inOpaque && (currByte == 0 || ((int) currPixNb + 1) % (int) m_size.w == 0)) || (!inOpaque && currByte != 0)) {
#endif
					LLNode* lnOpaqueInfo = new LLNode;
					lnOpaqueInfo->pData = new long;

					if (inOpaque) {
						if (currByte == 0) {
							*((long*) lnOpaqueInfo->pData) = currPixNb - 1;
						}
						else {
							*((long*) lnOpaqueInfo->pData) = currPixNb;
						}

#if TARGET_3DS
						m_nbZoneByLine[currPixNb / (int) m_size.h]++;
#else
                        m_nbZoneByLine[currPixNb / (int) m_size.w]++;
#endif
					}
					else {
						*((long*) lnOpaqueInfo->pData) = currPixNb;

#if TARGET_3DS
                        if (((int) currPixNb + 1) % (int) m_size.h == 0) {
                            doEndNow = true;
                        }
#else
                        if (((int) currPixNb + 1) % (int) m_size.w == 0) {
                            doEndNow = true;
                        }
#endif
					}

					addNodeToList(&lOpaqueInfo, lnOpaqueInfo);

					if (doEndNow) {
						LLNode* lnOpaqueInfo = new LLNode;
						lnOpaqueInfo->pData = new long;

						*((long*)lnOpaqueInfo->pData) = currPixNb;

						addNodeToList(&lOpaqueInfo, lnOpaqueInfo);

						doEndNow = false;
						inOpaque = false;
					}
					else {
						inOpaque = !inOpaque;
					}
				}
			}

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

	if (lOpaqueInfo.size % 2 == 1) {
		LLNode* lnOpaqueInfo = new LLNode;
		lnOpaqueInfo->pData = new long;

		*((long*)lnOpaqueInfo->pData) = currPixNb - 1;

#if TARGET_3DS
		m_nbZoneByLine[currPixNb / (int) m_size.h]++;
#else
        m_nbZoneByLine[currPixNb / (int) m_size.w]++;
#endif

		addNodeToList(&lOpaqueInfo, lnOpaqueInfo);
	}

	m_maskNbZone = lOpaqueInfo.size;
    m_mask = new long[m_maskNbZone];
    
    printf("NbZones: %u\n", m_maskNbZone);
    LLNode* currNode = lOpaqueInfo.pHead;
    
#if TARGET_3DS
	m_maskIdByLine = new long*[(int) m_size.w];
#else
    m_maskIdByLine = new long*[(int) m_size.h];
#endif

#if TARGET_3DS
	for (i = 0; i < m_size.w; i++) {
#else
    for (i = 0; i < m_size.h; i++) {
#endif
		m_maskIdByLine[i] = new long[m_nbZoneByLine[i]];
	}

	i = 0;
	j = 0;

#if TARGET_3DS
	int lastX = 0;
#else
    int lastY = 0;
#endif

	while (currNode != NULL) {
		long* pPosData = (long *)currNode->pData;

		if (i % 2 == 0) {
			m_mask[i] = *pPosData;

#if TARGET_3DS
			int currX = m_mask[i] / m_size.h;
            
            if (lastX != currX) {
                lastX = currX;
                j = 0;
            }
            
            m_maskIdByLine[currX][j] = i;
#else
            int currY = m_mask[i] / m_size.w;

            if (lastY != currY) {
                lastY = currY;
                j = 0;
            }
            
            m_maskIdByLine[currY][j] = i;
#endif

			j++;
		}
		else {
			m_mask[i] = *pPosData + 1 - m_mask[i - 1];
		}

        removeNodeFromList(&lOpaqueInfo, currNode);

        LLNode* nodeToDelete = currNode;
        currNode = currNode->pNext;
        
        delete nodeToDelete;
        delete pPosData;

        i++;
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

void Image::draw(drawbuffer* pBuffer, int dstX, int dstY, bool reversed, bool masked) {
    draw(pBuffer, dstX, dstY, 0, 0, (int) m_size.w, (int) m_size.h, reversed, masked);
}

void Image::draw(drawbuffer* pBuffer, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, bool reversed, bool masked) {
    unsigned int imgBufIdx;
	unsigned int overflowLeft = 0, overflowRight = 0, overflowTop = 0, overflowBottom = 0;
    int zoneSize;
    
    reversed = pBuffer->reverse;
    
    if (srcX >= m_size.w || srcY >= m_size.h) {
        return;
    }
    
    if (srcX >= 0 || srcY >= 0 || srcW > 0 || srcH > 0) {
        if (srcW > m_size.w || srcH > m_size.h) {
            return;
        }
        
        if ((srcX + srcW) > m_size.w) {
            srcW = m_size.w - srcX;
        }
        if ((srcY + srcH) > m_size.h) {
            srcH = m_size.h - srcY;
        }
    }
    else {
        srcW = m_size.w;
        srcH = m_size.h;
    }
    
	overflowLeft = maxInt(0, -dstX);
	overflowRight = maxInt(0, (dstX + srcW) - pBuffer->width);
	overflowTop = maxInt(0, -dstY);
	overflowBottom = maxInt(0, (dstY + srcH) - pBuffer->height);
    
	//printf("%d, %d, %d, %d, %d, %d, %d, %d\n", overflowLeft, overflowRight, overflowBottom, overflowTop, dstY, srcH, dstX, srcW);

	if (overflowLeft > srcW || overflowRight > srcW || overflowTop > srcH || overflowBottom > srcH) {
		return;
	}
    
    if (overflowLeft > 0) {
        srcX += overflowLeft;
        srcW -= overflowLeft;
        dstX = 0;
    }
    
  	if (masked) {
#if TARGET_3DS
        int dstYInv = (pBuffer->height - 1 - dstY);
        
		for (int x = srcX; x < srcX + srcW; x++) {
            
			for (int j = 0; j < m_nbZoneByLine[x]; j++) {
				int maskIdx = m_maskIdByLine[x][j];

				imgBufIdx = (unsigned int) m_mask[maskIdx];
				zoneSize  = (int)          m_mask[maskIdx + 1];

                int posOnSrcImgX = (srcX + x) * m_size.h;
                int posOnSrcImgY = imgBufIdx % (int) m_size.h;
                
                int initYTranspDelta = posOnSrcImgY - srcY;
                
                int newImgBufIdx = imgBufIdx;

				// Skipping out of bounds zones
                if (posOnSrcImgY + zoneSize < srcY || posOnSrcImgY >= srcY + srcH) {
                    continue;
                }
                
                // Custom clipping
                
				// Building coordinates
                int posOnBufferX = dstX + x - srcX;
                int posOnBufferY = dstYInv - srcH + initYTranspDelta;
                
                // Buffer edge clipping
                int overflowZoneTop = maxInt((posOnBufferY + zoneSize) - (pBuffer->height - 1), 0);
                int overflowZoneBottom = -minInt(posOnBufferY, 0);
                int newZoneSize = zoneSize - overflowZoneTop - overflowZoneBottom;
                
                if (newZoneSize > 0) {
                    posOnBufferY += overflowZoneBottom;
                    newImgBufIdx += overflowZoneBottom;

                    // Blittin'
                    memcpy(pBuffer->buffer + (posOnBufferY * SCREEN_BPP) + (posOnBufferX * pBuffer->height * SCREEN_BPP),
                        m_pImgData + newImgBufIdx * SCREEN_BPP,
                        newZoneSize * SCREEN_BPP);
                }
			}
		}
#else
        for (int y = srcY; y < srcY + srcH; y++) {
            int reversedY = (m_size.h - 1) - (m_size.h - 1 - srcY) + (y % srcH);
            
            if (dstY + ((srcH-1) - (y % srcH)) < 0 || dstY + ((srcH-1) - (y % srcH)) > pBuffer->height-1) {
                continue;
            }
            
            for (int j = 0; j < m_nbZoneByLine[reversedY]; j++) {
                int maskIdx = m_maskIdByLine[reversedY][j];
                
                imgBufIdx = (unsigned int) m_mask[maskIdx];
                zoneSize  = (int)          m_mask[maskIdx + 1];
                
                int posOnImgX = (int) imgBufIdx % (int) m_size.w;
                int posOnImgY = (srcH - 1) - (imgBufIdx / m_size.w);
                
                // Skipping out of bounds zones
                if (posOnImgX + zoneSize < srcX || posOnImgX > srcX + srcW || (posOnImgX - srcX) + zoneSize + dstX < 0 || (posOnImgX - srcX) + dstX > pBuffer->width-1) {
                    continue;
                }
                
                // Custom clipping
                int newPosOnImgX = minInt(maxInt(posOnImgX, srcX), srcX + srcW);
                int newPosOnImgXDelta = (newPosOnImgX - posOnImgX);
                
                int newZoneSize = minInt((posOnImgX + zoneSize), srcX + srcW) - posOnImgX - newPosOnImgXDelta;
                int newImgBufIdx = imgBufIdx + newPosOnImgXDelta;
                
                // Buffer edge clipping
                int transpZoneX = maxInt(-dstX - (newPosOnImgX - srcX), 0);
                
                if (dstX < 0) {
                    newPosOnImgX += transpZoneX;
                    newZoneSize = maxInt(0, newZoneSize - transpZoneX);
                    newImgBufIdx += transpZoneX;
                }
                else if (dstX + (newPosOnImgX - srcX) + newZoneSize > pBuffer->width-1) {
                    newZoneSize = maxInt(0, newZoneSize - (dstX + (newPosOnImgX - srcX) + newZoneSize - pBuffer->width));
                }
                
                // Building final coordinates
                int posOnBufferX = (newPosOnImgX + dstX - srcX);
                
                int posOnBufferY;
                // TODO: OK FOR NOW THE FLAG REVERSED BLIT THE SPRITE FROM UP TO DOWN.
                // TODO: Following a logic of blitting the image on a regular buffer and not a framebuffer which y is inverted.
                // TODO: Gotta fix that later
                if (!reversed) {
                    //posOnBufferY = (dstY + y);
                    posOnBufferY = (pBuffer->height - srcH - dstY + y - srcY);
                }
                else {
                    posOnBufferY = (((srcH - 1 + srcY) - reversedY) + dstY);
                }
                
                // Blittin'
                memcpy(pBuffer->buffer + (posOnBufferX * SCREEN_BPP) + (posOnBufferY * pBuffer->width * SCREEN_BPP),
                       m_pImgData + newImgBufIdx * SCREEN_BPP,
                       newZoneSize * SCREEN_BPP);
            }
        }
#endif
	}
	else {
#if TARGET_3DS
		for (int i = overflowLeft; i < srcW - overflowRight; i++) {
			memcpy(pBuffer->buffer + (int) (((pBuffer->height - 1) - (srcH - 1) - dstY) * SCREEN_BPP) + ((dstX + i) * pBuffer->height * SCREEN_BPP),
                m_pImgData + (int) (srcY * SCREEN_BPP + (i + srcX) * m_size.h * SCREEN_BPP),
				(srcH - overflowTop - overflowBottom) * SCREEN_BPP);
		}
#else
        // TODO: ui c'est rough grosse répétition de code heh
        if (!reversed) {
            for (int i = overflowTop; i < srcH - overflowBottom; i++) {
                memcpy(
                       pBuffer->buffer + (maxInt(0, dstX) * SCREEN_BPP) + ((maxInt(0, dstY) + i - overflowTop) * pBuffer->width * SCREEN_BPP),
                       m_pImgData + (int) (((i + srcY)) * (m_size.w * SCREEN_BPP)) + ((srcX) * SCREEN_BPP),
                       (srcW - overflowRight) * SCREEN_BPP);
            }
        }
        else {
            for (int i = overflowTop; i < srcH - overflowBottom; i++) {
                memcpy(
                       pBuffer->buffer + (maxInt(0, dstX) * SCREEN_BPP) + ((maxInt(0, dstY) + i - overflowTop) * pBuffer->width * SCREEN_BPP),
                       m_pImgData + (int) ((m_size.h - (i + srcY) - 1) * (m_size.w * SCREEN_BPP)) + ((srcX) * SCREEN_BPP),
                       (srcW - overflowRight) * SCREEN_BPP);
            }
        }
#endif
	}
}
