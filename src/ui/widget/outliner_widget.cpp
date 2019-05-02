#include "outliner_widget.hpp"

#include "../../core/system.hpp"
#include "../../manager/rsc_manager.hpp"


RowWidget::RowWidget(int x, int y, int w, int h, IWidget* pWidget, RowWidget* pParentRowWidget)
	: IWidget(x, y, w, h),
	m_hoverColor(50, 50, 50) {

	m_pWidget = pWidget;
	m_pParentRowWidget = pParentRowWidget;

	m_pTextWidget = new Text(m_pWidget->getName(), RscManager::get()->getFontRsc("font-small"), x, y);

	addChildWidget(m_pTextWidget);
}

RowWidget::~RowWidget() {

}

void RowWidget::update() {
	updateChildren();

	if (m_bIsPressed) {
		Input* pInputSys = System::get()->getInputSys();

		if (!pInputSys->IsButtonPressed(MOUSE_BTN_LEFT) && m_bIsPressed) {
			onRelease();
		}
	}

	if (m_bIsHovered) {
		Input* pInputSys = System::get()->getInputSys();
		vect2d_t vMousePos = pInputSys->getCurrInputPos();

		if (!m_rect.isPointInRect(vMousePos.x, vMousePos.y)) {
			m_bIsHovered = false;
		}

	}

	m_bPressedThisLoop = false;
}

bool RowWidget::receiveTouchInput(vect2d_t touchPt) {
	if (m_rect.isPointInRect(touchPt.x, touchPt.y)) {
		onPress();
		m_bPressedThisLoop = true;

		return true;
	}
	else {
		return false;
	}
}

void RowWidget::draw(uint8* pBuffer) {
	if(m_bIsHovered) {
		vect2df_t vPos = m_rect.getPos();
		size2df_t sSize = m_rect.getSize();
		drawBox(pBuffer, vPos.x, vPos.y, vPos.x + sSize.w, vPos.y + sSize.h, &m_hoverColor);
	}

	drawChildren(pBuffer);
}

void RowWidget::onPress() {
	if (m_bIsPressed) {
		printf("%s\n", m_pWidget->getName());
		return;
	}

	m_bIsPressed = true;
}

void RowWidget::onRelease() {
	m_bIsPressed = false;
}

void RowWidget::onHover() {
	m_bIsHovered = true;
}

bool RowWidget::isHovered() {
	return m_bIsHovered;
}


// ---------------------------------------------------------------------------------


OutlinerWidget::OutlinerWidget(int x, int y, int w, int h, Scene* pRootScene)
	: IWidget(x, y, w, h) {

	initList(&m_llListRows);
	m_pRootScene = pRootScene;
	populate();
}

OutlinerWidget::~OutlinerWidget() {

}

void OutlinerWidget::populate() {
	for (int i = 0; i < m_pRootScene->getComponentCount(); i++) {
		RowWidget* rowWidget = new RowWidget(0, 8*i, m_rect.getSize().w, 7, m_pRootScene->getNComponent(i), NULL);
		rowWidget->setParentWidget(this);
		addDataToList(&m_llListRows, rowWidget);
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

	Input* pInputSys = System::get()->getInputSys();
	vect2d_t vMousePos = pInputSys->getCurrInputPos();

	if (m_rect.isPointInRect(vMousePos.x, vMousePos.y)) {
		LLNode* pCurrNode = m_llListRows.pHead;

		while (pCurrNode != NULL) {
			RowWidget* rowWidget = (RowWidget*)pCurrNode->pData;
			if (!rowWidget->isHovered() && rowWidget->getRect()->isPointInRect(vMousePos.x, vMousePos.y)) {

				rowWidget->onHover();
				break;
			}
			pCurrNode = pCurrNode->pNext;
		}
	}

	if (m_bIsPressed) {
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
