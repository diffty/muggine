#include "text.hpp"


Text::Text(const char* szText, IFont* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	init(szText, pFont, vPos.x, vPos.y);
}

Text::Text(const char* szText, IFont* pFont, float fXPos, float fYPos)
	: IWidget(fXPos, fYPos, 1, 1) {

	init(szText, pFont, fXPos, fYPos);
}

Text::Text(const wchar_t* szText, IFont* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	init(szText, pFont, vPos.x, vPos.y);
}

Text::Text(const wchar_t* szText, IFont* pFont, float fXPos, float fYPos)
	: IWidget(fXPos, fYPos, 1, 1) {

	init(szText, pFont, fXPos, fYPos);
}

Text::Text(int iNum, FontBitmap* pFont, vect2df_t vPos)
	: IWidget(vPos.x, vPos.y, 1, 1) {

	init(iNum, pFont, vPos.x, vPos.y);
}

Text::Text(int iNum, FontBitmap* pFont, float fXPos, float fYPos)
	: IWidget(fXPos, fYPos, 1, 1) {

	init(iNum, pFont, fXPos, fYPos);
}

Text::~Text() {
	delete m_szText;
}

void Text::init(const char* szText, IFont* pFont, float fXPos, float fYPos) {
	int iTextSize = strlen(szText);
	wchar_t* wideStr = new wchar_t[iTextSize+1];
	mbsrtowcs(wideStr, &szText, iTextSize+1, NULL);
	init(wideStr, pFont, fXPos, fYPos);
}

void Text::init(const wchar_t* szText, IFont* pFont, float fXPos, float fYPos) {
    m_pFont = pFont;
    m_szText = new wchar_t[1];
    m_szText[0] = '\0';
    
    setText(szText);
}

void Text::init(int iNum, FontBitmap* pFont, float fXPos, float fYPos) {
    m_pFont = pFont;
    m_szText = new wchar_t[1];
    m_szText[0] = '\0';
    
	wchar_t* newText = intToStr(iNum);
    setText((wchar_t*) newText);
    delete newText;
}

wchar_t* Text::getText() {
	return m_szText;
}

void Text::setText(const char* szText) {
	int iTextSize = strlen(szText);
	wchar_t* wideStr = new wchar_t[iTextSize + 1];
	mbsrtowcs(wideStr, &szText, iTextSize + 1, NULL);
	setText(wideStr);
}

void Text::setText(const wchar_t* szText) {
	long lTextLen = wcslen(szText);

	if (m_szText) delete m_szText;

	m_szText = new wchar_t[lTextLen + 1];

	wcscpy(m_szText, szText);

	updateSize();
}

void Text::setText(int iNum, int padding) {
	wchar_t* newText = intToStr(iNum, padding);
	setText(newText);
	delete newText;
}


int Text::getCharHeight() {
	return m_pFont->getCharHeight();
}

void Text::updateSize() {
	int i = 0;
	int sizeW = 0;
	int currSizeW = 0;
	long sizeH = getCharHeight();

	char c;

	while ((c = m_szText[i]) != '\0') {
		if (c == '\n') {
			sizeH += sizeH;

			if (sizeW < currSizeW) sizeW = currSizeW;

			currSizeW = 0;
		}
		else {
			currSizeW += m_pFont->getWidthForChar(c, 16);
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

void Text::drawChar(uint8* buffer, float x, float y, wchar_t c) {
	Color* testcolor = new Color(255, 255, 0);

	m_pFont->draw(buffer, (wchar_t) c, x, y, 16, testcolor);

	delete testcolor;
}

void Text::drawStr(uint8* buffer, float x, float y, wchar_t* text) {
	int i = 0;
	int iDrawCurW = 0;
	int iDrawCurH = 0;

	wchar_t c;

	while ((c = m_szText[i]) != '\0') {
		if (c == '\n') {
			iDrawCurW = 0;
			iDrawCurH += m_pFont->getCharHeight();
		}
		else {
			drawChar(buffer, x + iDrawCurW, y + iDrawCurH, c);
			iDrawCurW += m_pFont->getWidthForChar(c, 16);
		}

		i++;
	}
}

// Grosse source de memory leaks AVEREE cette merde. Ne pas oublier de delete après usage
wchar_t* Text::intToStr(int iNum, int padding) {
	unsigned int i, j;
	char tmp;
	wchar_t* newStr = new wchar_t[16];

	if (iNum == 0) {
        if (padding > 0) {
            int k;
            for (k = 0; k < padding; k++) {
                newStr[k] = 48;
            }
            newStr[k] = '\0';
        }
        else {
            newStr[0] = '0';
            newStr[1] = '\0';
        }
		return newStr;
	}

	for (i = 0; iNum > 0; i++) {
		newStr[i] = (iNum % 10) + 48;

		iNum /= 10;
	}

	//newStr[i] = '\0';
    
    int k;
    
    int iOverNumber = maxInt(0, padding - i);
    for (k = 0; k < iOverNumber; k++) {
        newStr[k+1] = newStr[k];
        newStr[k] = 48;
    }
    
	for (j = 0; j < i*0.5; j++) {
		tmp = newStr[k + i - 1 - j];
		newStr[k + i - 1 - j] = newStr[j];
		newStr[k + j] = tmp;
	}
    
    newStr[iOverNumber + i] = '\0';
    
	return newStr;
}
