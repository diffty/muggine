#include "outliner_widget.hpp"
#include "system.hpp"
#include "rsc_manager.hpp"


WidgetRow::WidgetRow(int x, int y, int w, int h, IWidget* pWidget, WidgetRow* pParentRowWidget)
	: IWidget(x, y, w, h) {

	m_pWidget = pWidget;
	m_pParentRowWidget = pParentRowWidget;

	m_pTextWidget = new Text(m_pWidget->getName(), RscManager::get()->getFontRsc("font-small"), x, y);

	addChildWidget(m_pTextWidget);
}

WidgetRow::~WidgetRow() {

}

void WidgetRow::update() {
	updateChildren();

	if (m_bIsPressed) {
		Input* pInputSys = System::get()->getInputSys();

		if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
			onRelease();
		}
	}

	m_bPressedThisLoop = false;
}

bool WidgetRow::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;

		return true;
	}
	else {
		return false;
	}
}

void WidgetRow::draw(uint8* pBuffer) {
	drawChildren(pBuffer);
}

void WidgetRow::onPress() {
	if (m_bIsPressed) {
		printf("%s\n", m_pWidget->getName());
		return;
	}

	m_bIsPressed = true;
}

void WidgetRow::onRelease() {
	m_bIsPressed = false;
}


// ---------------------------------------------------------------------------------


OutlinerWidget::OutlinerWidget(int x, int y, int w, int h, Scene* pRootScene)
	: IWidget(x, y, w, h) {

	m_pRootScene = pRootScene;
	populate();
}

OutlinerWidget::~OutlinerWidget() {

}

void OutlinerWidget::populate() {
	for (int i = 0; i < m_pRootScene->getComponentCount(); i++) {
		WidgetRow* rowWidget = new WidgetRow(0, 7*i, 100, 7, m_pRootScene->getNComponent(i), NULL);
		addChildWidget(rowWidget);
	}
}

void OutlinerWidget::onPress() {
	if (m_bIsPressed) {
		return;
	}

	m_bIsPressed = true;
}

void OutlinerWidget::onRelease() {
	m_bIsPressed = false;
}

void OutlinerWidget::update() {
	updateChildren();

	if (m_bIsPressed) {
		Input* pInputSys = System::get()->getInputSys();
		vect2d_t vMousePos = pInputSys->getCurrInputPos();

		vect2df_t vPos = m_rect.getPos();
		size2df_t sSize = m_rect.getSize();

		if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
			onRelease();
		}
	}

	m_bPressedThisLoop = false;
}

bool OutlinerWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		//onPress();
		//m_bPressedThisLoop = true;

		receiveTouchInputChildren(touchPt);

		return true;
	}
	else {
		return false;
	}
}

void OutlinerWidget::draw(uint8* buffer) {
	if (m_bIsActive) {
		drawChildren(buffer);
	}
}


void drawElementRow(IWidget* pWidget, int level = 0) {

}
