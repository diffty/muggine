#include "ts_text_bubble.hpp"



TextBubble::TextBubble(char* szText, Font* pFont, float x, float y, float w, float h) :
	IWidget(x, y, w, h),
	m_cBorderColor(0, 0, 0),
	m_cFillColor(255, 255, 255) {

	vect2df_t textPos;
	textPos.x = x + 2;
	textPos.y = y + 2;

	m_pTextWidget = new Text(szText, pFont, textPos);

	// setText(szText);

	m_szText = new char[1];
	m_szText[0] = '\0';
}


TextBubble::~TextBubble() {
	delete m_pTextWidget;
}

char* TextBubble::getText() {
	return m_szText;
}

void TextBubble::setText(char* szText) {
	delete m_szText;

	m_szText = new char[strlen(szText) + 1];

	strcpy(m_szText, szText);

	m_pTextWidget->setText(m_szText);

	updateSize();
}

void TextBubble::translate(int x, int y, ETransformMode transformMode) {
	IWidget::translate(x, y, transformMode);

	m_pTextWidget->translate(x + 2, y + 1, transformMode);
}

void TextBubble::updateSize() {
	size2df_t sTextWidgetSize = m_pTextWidget->getRect()->getSize();
	getRect()->setSize(sTextWidgetSize.w + 5, sTextWidgetSize.h + 5);
}

void TextBubble::draw(uint8* fb) {
	if (m_bIsActive) {
		vect2df_t vPos = getRect()->getPos();
		size2df_t sSize = getRect()->getSize();

		drawBox(fb, vPos.x, vPos.y, vPos.x + sSize.w, vPos.y + sSize.h, &m_cBorderColor);
		drawBox(fb, vPos.x+1, vPos.y+1, vPos.x + sSize.w - 1 , vPos.y + sSize.h - 1, &m_cFillColor);

		m_pTextWidget->draw(fb);
	}
}