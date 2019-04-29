#ifndef ANIMATIONTIMELINE_HPP
#define ANIMATIONTIMELINE_HPP

#include "double_linked_list.hpp"
#include "animationevent.hpp"


class AnimationTimeline {
public:
	AnimationTimeline();
	~AnimationTimeline();

	void addEvent(AnimationEvent* pAnimEvent);
	DLLNode* getDLLNodeBeforeTime(float fTime);
	DLLNode* getDLLNodeBeforeTime(float fTime, DLLNode* pStartDLLNode);
	DLLNode* getDLLNodeBeforeCurrent();
	DLLNode* getDLLNodeAfterCurrent();
	AnimationEvent* getEventBeforeTime(float fTime);
	AnimationEvent* getEventBeforeCurrent();
	AnimationEvent* getEventAfterCurrent();
	float getTime();
	bool isPlaying();
	void setTime(float fTime);
	void setIsPlaying(bool bNewIsPlaying);
	void update();
	void updateEventWidget(DLLNode* pAnimEventDLLNode);

private:
	float m_fCurrTime = 0.0;
	float m_fPlaySpeed = 1.0;
	bool m_bIsPlaying = false;

	DLLNode* m_pCurrAnimDLLNode = NULL;
	AnimationEvent* m_pCurrAnimEvent = NULL;

	DoubleLinkedList m_dllAnimEventList;
};


#endif
