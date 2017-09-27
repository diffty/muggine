#include "iwidget.hpp"
#include "scene.hpp"



void IWidget::translate(float x, float y, ETransformMode transformMode) {
	vect2df_t currPos = m_rect.getPos();
	if (transformMode == TRANSFORM_REL) {
		currPos.x += x;
		currPos.y += y;
	}
	else {
		currPos.x = x;
		currPos.y = y;
	}
	m_rect.setPos(currPos.x, currPos.y);
};

void IWidget::setActive(bool bIsActive) {
	m_bIsActive = bIsActive;
};

bool IWidget::isActive() {
	return m_bIsActive;
};

void IWidget::setParentScene(Scene* pParentScene) {
	m_pParentScene = pParentScene;
}

Scene* IWidget::getParentScene() {
	return m_pParentScene;
}