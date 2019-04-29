#include "animationevent.hpp"


AnimationEvent::AnimationEvent(float fTime, IWidget* pWidget, vect2df_t vPosition, EAnimationEventType eType) {
	m_fTime = fTime;
	m_eType = eType;
	m_pWidget = pWidget;
	m_vPosition = vPosition;
}

AnimationEvent::~AnimationEvent() {

}

float AnimationEvent::getTime() {
	return m_fTime;
}

IWidget* AnimationEvent::getWidget() {
	return m_pWidget;
}

EAnimationEventType AnimationEvent::getEventType() {
	return m_eType;
}

vect2df_t AnimationEvent::getPosition() {
	return m_vPosition;
}
