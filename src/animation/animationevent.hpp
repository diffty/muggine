#ifndef ANIMATIONEVENT_HPP
#define ANIMATIONEVENT_HPP

#include "../core/iwidget.hpp"
#include "../core/common_types.hpp"


enum EAnimationEventType {
	EAnimationEventType_STEP,
	EAnimationEventType_LINEAR
};


class AnimationEvent {
public:
	AnimationEvent(float fTime, IWidget* pWidget, vect2df_t vPosition, EAnimationEventType eType = EAnimationEventType_STEP);
	~AnimationEvent();

	float getTime();
	IWidget* getWidget();
	EAnimationEventType getEventType();
	vect2df_t getPosition();

private:
	float m_fTime;
	IWidget* m_pWidget;
	EAnimationEventType m_eType;
	vect2df_t m_vPosition;
};


#endif
