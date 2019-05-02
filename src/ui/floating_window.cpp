#include "floating_window.hpp"

#include "titlebar.hpp"


FloatingWindow::FloatingWindow(int x, int y, int w, int h, IWidget* pContentWidget)
	: IWidget(x, y, w, h),
	m_bgColor(10, 10, 10) {

	if (pContentWidget != NULL) {
		setContent(pContentWidget);
	}

	m_pTitlebar = new Titlebar("floating window", this);
	m_pTitlebar->setParentWidget(this);
	m_pTitlebar->translate(x, y, TRANSFORM_ABS);

	m_pTitlebar->addButton(
		RscManager::get()->getSprShtRsc("ui"),
		{ 1, 1 },
		0,
		1,
		onCloseBtnPressed,
		(void*)this);
}

FloatingWindow::~FloatingWindow()
{

}

void FloatingWindow::setContent(IWidget* pWidget) {
	if (m_pContentWidget != NULL)
		m_pContentWidget->setParentWidget(NULL);

	vect2df_t sWidgetPos = getRect()->getPos();
	size2df_t sWidgetSize = getRect()->getSize();

	m_pContentWidget = pWidget;
	m_pContentWidget->translate(sWidgetPos.x, sWidgetPos.y + UI_TITLEBAR_HEIGHT, TRANSFORM_ABS);
	m_pContentWidget->getRect()->setSize(sWidgetSize.w, sWidgetSize.h - UI_TITLEBAR_HEIGHT);

	pWidget->setParentWidget(this);

	m_pContentWidget->translate(0, UI_TITLEBAR_HEIGHT, TRANSFORM_REL);
}

void FloatingWindow::update() {
	updateChildren();
}

void FloatingWindow::draw(uint8* pBuffer) {
	vect2df_t vPos = m_rect.getPos();
	size2df_t sSize = m_rect.getSize();
	drawBox(pBuffer, vPos.x, vPos.y, vPos.x + sSize.w, vPos.y + sSize.h, &m_bgColor);
	drawChildren(pBuffer);
}

void FloatingWindow::close() {
	setParentWidget(NULL);
}

void FloatingWindow::onCloseBtnPressed(void* pWindow) {
	((FloatingWindow*) pWindow)->close();
}

void FloatingWindow::onMove(vect2d_t vDeltaPos) {
	translate(vDeltaPos.x, vDeltaPos.y, TRANSFORM_REL);
}
