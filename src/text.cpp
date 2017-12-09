#include "text.hpp"



Text::Text(char* szText, Font* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	m_pFont = pFont;
	m_szText = new char[1];
	m_szText[0] = '\0';

	setText(szText);
}

Text::Text(int iNum, Font* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	m_pFont = pFont;
    m_szText = new char[1];
    m_szText[0] = '\0';

	char* newText = intToStr(iNum);
	setText(newText);
	delete newText;
}

Text::~Text() {
	delete m_szText;
}

char* Text::getText() {
	return m_szText;
}

void Text::setText(char* szText) {
	int iTextLen = strlen(szText);

	if (m_szText) delete m_szText;

	m_szText = new char[iTextLen+1];

	strcpy(m_szText, szText);

	updateSize();
}

void Text::setText(int iNum) {
	char* newText = intToStr(iNum);
	setText(newText);
	delete newText;
}

void Text::updateSize() {
	int i = 0;
	int sizeW = 0;
	int currSizeW = 0;
	int sizeH = m_pFont->getFrameSize().h;

	char c;

	while ((c = m_szText[i]) != '\0') {
		if (c == '\n') {
			sizeH += m_pFont->getFrameSize().h;

			if (sizeW < currSizeW) sizeW = currSizeW;

			currSizeW = 0;
		}
		else {
			currSizeW += m_pFont->getSizeForChar(c);
		}

		i++;
	}

	if (sizeW < currSizeW) sizeW = currSizeW;

	getRect()->setSize(sizeW, sizeH);
}

void Text::draw(uint8* buffer) {
	if (m_bIsActive)
		drawChildren(buffer);
		
		drawStr(buffer, m_rect.getPos().x, m_rect.getPos().y, m_szText);
}

void Text::drawChar(uint8* buffer, float x, float y, char c) {
	m_pFont->draw(buffer, c - 32, x, y, false, true);
}

void Text::drawStr(uint8* buffer, float x, float y, char* text) {
	int i = 0;
	int iDrawCurW = 0;
	int iDrawCurH = 0;

	char c;

	while ((c = m_szText[i]) != '\0') {
		if (c == '\n') {
			iDrawCurW = 0;
			iDrawCurH += m_pFont->getFrameSize().h;
		}
		else {
			drawChar(buffer, x + iDrawCurW, y + iDrawCurH, c);
			iDrawCurW += m_pFont->getSizeForChar(c);
		}

		i++;
	}
}

// Grosse source de memory leaks AVEREE cette merde
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
