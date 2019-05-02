#include "font_ttf.hpp"


FontTTF::FontTTF(char* szFileName) {
	FILE* fp = fopen(szFileName, "rb");  // "c:/windows/fonts/arialbd.ttf"
	
	// Getting file size
	fseek(fp, 0, SEEK_END);
	int iFileSize = ftell(fp);
	rewind(fp);

	// Reading file
	unsigned char* ttf_buffer = new unsigned char[iFileSize];
	fread(ttf_buffer, 1, iFileSize, fp);
	fclose(fp);
	
	// Loading font from buffer
	stbtt_InitFont(&m_font, (const unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

	delete ttf_buffer;

}

FontTTF::~FontTTF() {
	
}

void FontTTF::draw(uint8* fb, char c, int x, int y, int size) {
	int w, h;
	uint8* pGlyphBitmap = stbtt_GetCodepointBitmap(&m_font, 0, stbtt_ScaleForPixelHeight(&m_font, size), c, &w, &h, 0, 0);
	
	/* uint8* screenBitmap = new uint8[w*h*SCREEN_BPP];
	for (int i = 0; i < w*h; i++) {
		memset(screenBitmap + i * SCREEN_BPP, bitmap[i], SCREEN_BPP);
	}*/

	// If drawing line by line without bothering with alpha (i = y)
	//memcpy(fb + (SCREEN_WIDTH * i) * SCREEN_BPP, pGlyphBitmap + i * w * SCREEN_BPP, w * SCREEN_BPP);

	for (int i = 0; i < w*h; i++) {
		uint8 glyphPixelValue = *(pGlyphBitmap + i * w * SCREEN_BPP);
		unsigned int screenIdx = ((x + i % w) + ((y + i / w) * SCREEN_WIDTH));
		uint8 currR = *(fb + screenIdx * SCREEN_BPP);
		uint8 currG = *(fb + screenIdx * SCREEN_BPP + 1);
		uint8 currB = *(fb + screenIdx * SCREEN_BPP + 2);

		*(fb + screenIdx * SCREEN_BPP) = currR * ((255.0 - float(glyphPixelValue)) / 255.0);
		*(fb + screenIdx * SCREEN_BPP + 1) = currG * ((255.0 - float(glyphPixelValue)) / 255.0);
		*(fb + screenIdx * SCREEN_BPP + 2) = currB * ((255.0 - float(glyphPixelValue)) / 255.0);
	}
}

int FontTTF::getSizeForChar(char c) {
	return 0; //return m_pCharSizesTable[c - 32];
}
