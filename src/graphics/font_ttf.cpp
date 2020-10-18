#include "font_ttf.hpp"

#include "../utils/math_tools.hpp"


FontTTF::FontTTF(const char* szFileName)
	: IFont() {

	FILE* fp = fopen(szFileName, "rb");  // "c:/windows/fonts/arialbd.ttf"
	
	// Getting file size
	fseek(fp, 0, SEEK_END);
	int iFileSize = ftell(fp);
	rewind(fp);

	// Reading file
	m_pFontFileBuffer = new unsigned char[iFileSize];
	fread(m_pFontFileBuffer, 1, iFileSize, fp);
	
	// Loading font from buffer
	if (!stbtt_InitFont(&m_font, m_pFontFileBuffer, stbtt_GetFontOffsetForIndex(m_pFontFileBuffer, 0))) {
		printf("Error loading font\n");
	}

	fclose(fp);
}

FontTTF::~FontTTF() {
	delete m_pFontFileBuffer;
}

void FontTTF::draw(drawbuffer* pBuffer, char c, int x, int y, int size, Color* color) {
	draw(pBuffer, c - 32, x, y, size, color);
}

void FontTTF::draw(drawbuffer* pBuffer, int codepoint, int x, int y, int hSize, Color* color) {
	int w, h, xoff, yoff;
	uint8* pGlyphBitmap = stbtt_GetCodepointBitmap(
		&m_font,
		0,
		stbtt_ScaleForPixelHeight(&m_font, hSize),
		codepoint,
		&w, &h,
		&xoff, &yoff
	);
	
	/* uint8* screenBitmap = new uint8[w*h*SCREEN_BPP];
	for (int i = 0; i < w*h; i++) {
		memset(screenBitmap + i * SCREEN_BPP, bitmap[i], SCREEN_BPP);
	}*/

	// If drawing line by line without bothering with alpha (i = y)
	//memcpy(fb + (SCREEN_WIDTH * i) * SCREEN_BPP, pGlyphBitmap + i * w * SCREEN_BPP, w * SCREEN_BPP);

	color_t colorStruct = color->getColor();

	for (int i = 0; i < w*h; i++) {
		float glyphPixelValue = float(*(pGlyphBitmap + i)) / 255.0;
		unsigned int screenIdx = ((x + xoff + i % w) + ((y + yoff + i / w) * SCREEN_WIDTH));
		uint8 currR = *(pBuffer->buffer + screenIdx * SCREEN_BPP);
		uint8 currG = *(pBuffer->buffer + screenIdx * SCREEN_BPP + 1);
		uint8 currB = *(pBuffer->buffer + screenIdx * SCREEN_BPP + 2);

		*(pBuffer->buffer + screenIdx * SCREEN_BPP)     = int(lerpf(currR, colorStruct.b, glyphPixelValue));
		*(pBuffer->buffer + screenIdx * SCREEN_BPP + 1) = int(lerpf(currG, colorStruct.g, glyphPixelValue));
		*(pBuffer->buffer + screenIdx * SCREEN_BPP + 2) = int(lerpf(currB, colorStruct.r, glyphPixelValue));
	}

	delete pGlyphBitmap;
}

int FontTTF::getWidthForChar(int codepoint, int hSize) {
	//stbtt_GetCodepointHMetrics(&m_font, c, &advance, &iLeftBearing);
	int iGlyphWidth, iLeftBearing, ascent, advance, lsb;
	float scale = stbtt_ScaleForPixelHeight(&m_font, hSize);
	stbtt_GetFontVMetrics(&m_font, &ascent, 0, 0);
	stbtt_GetCodepointHMetrics(&m_font, codepoint, &advance, &lsb);
	//int baseline = (int)(ascent*scale);

	return scale * advance;
}

int FontTTF::getCharHeight() {
	return 16;
}
