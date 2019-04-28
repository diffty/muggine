#include "animationevent.hpp"


AnimationEvent::AnimationEvent(float fTime, IWidget* pWidget, EAnimationEventType eType) {
	m_fTime = fTime;
	m_eType = eType;
	m_pWidget = pWidget;
}

AnimationEvent::~AnimationEvent() {

}
