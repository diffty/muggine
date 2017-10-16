#include "text.hpp"



Text::Text(char* szText, Font* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	m_pFont = pFont;
	m_szText = new char[1];
	m_szText[0] = '\0';
	setText(szText);
}


Text::~Text() {

}

char* Text::getText() {
	return m_szText;
}

void Text::setText(char* szText) {
	int iTextLen = strlen(szText);

	delete m_szText;
	m_szText = new char[iTextLen+1];

	for (int i = 0; i < iTextLen; i++) {
		m_szText[i] = szText[i];
	}

	m_szText[iTextLen] = '\0';
}

void Text::draw(uint8* buffer) {
	drawStr(buffer, m_rect.getPos().x, m_rect.getPos().y, m_szText);
}

void Text::drawChar(uint8* buffer, float x, float y, char c) {
	m_pFont->draw(buffer, c - 32, x, y, false, true);
}

void Text::drawStr(uint8* buffer, float x, float y, char* text) {
	int i = 0;

	while (text[i] != '\0') {
		drawChar(buffer, x + (i * 8), y, text[i]);
		i++;
	}
}

// Grosse source de memory leaks cette merde
char* Text::intToStr(int iNum) {
	unsigned int i, j;
	char tmp;
	char* newStr = new char[16];

	if (iNum == 0) {
		newStr[0] = '0';
		newStr[1] = '\0';
		return newStr;
	}

	for (i = 0; iNum > 0; i++) {
		newStr[i] = (iNum % 10) + 48;

		iNum /= 10;
	}

	newStr[i] = '\0';

	for (j = 0; j < i*0.5; j++) {
		tmp = newStr[i - 1 - j];
		newStr[i - 1 - j] = newStr[j];
		newStr[j] = tmp;
	}

	return newStr;
}