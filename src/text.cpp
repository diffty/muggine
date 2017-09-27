#include "text.hpp"



Text::Text(char* aText, Font* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	m_pFont = pFont;
	m_aText = aText;
}


Text::~Text() {

}

void Text::draw(uint8* buffer) {
	drawStr(buffer, m_rect.getPos().x, m_rect.getPos().y, m_aText);
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
char** Text::intToStr(int iNum) {
	unsigned int i, j;
	char tmp;
	char* newStr = (char *) malloc(16 * sizeof(char));

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

	return &newStr;
}