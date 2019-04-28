#ifndef ANIMATIONEVENT_HPP
#define ANIMATIONEVENT_HPP

#include "iwidget.hpp"


enum EAnimationEventType {
	EAnimationEventType_STEP,
	EAnimationEventType_LINEAR
};


class AnimationEvent {
public:
	AnimationEvent(float fTime, IWidget* pWidget, EAnimationEventType eType = EAnimationEventType_STEP);
	~AnimationEvent();

private:
	float m_fTime;
	IWidget* m_pWidget;
	EAnimationEventType m_eType;
};


#endif
