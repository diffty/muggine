#include "animationtimeline.hpp"

#include "system.hpp"


AnimationTimeline::AnimationTimeline() {
}

AnimationTimeline::~AnimationTimeline() {

}

void AnimationTimeline::addEvent(AnimationEvent* pAnimEvent) {
	DLLNode* pNewNode = m_dllAnimEventList.addData(pAnimEvent);
	
	/*if (m_pCurrAnimEvent == NULL) {
		m_pCurrAnimEvent = pAnimEvent;
	}
	if (m_pCurrAnimDLLNode == NULL) {
		m_pCurrAnimDLLNode = pNewNode;
	}*/
}

DLLNode* AnimationTimeline::getDLLNodeBeforeTime(float fTime) {
	DLLNode* pCurrNode = m_dllAnimEventList.getHead();
	DLLNode* pPrevNode = NULL;

	while (pCurrNode != NULL) {
		AnimationEvent* pCurrEvent = (AnimationEvent*)pCurrNode->pData;

		if (fTime < pCurrEvent->getTime())
			return pPrevNode;

		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}
	return pPrevNode;
}

DLLNode* AnimationTimeline::getDLLNodeBeforeTime(float fTime, DLLNode* pStartDLLNode) {
	if (pStartDLLNode == NULL) {
		pStartDLLNode = m_dllAnimEventList.getHead();
	}

	if (pStartDLLNode == NULL) {
		return NULL;
	}

	DLLNode* pCurrNode = pStartDLLNode;
	DLLNode* pPrevNode = pStartDLLNode->pPrev;

	bool bGoForward = true;

	AnimationEvent* pCurrEvent = (AnimationEvent*)pStartDLLNode->pData;

	if (pCurrEvent->getTime() > fTime) {
		bGoForward = false;

		if (pPrevNode == NULL) {
			return NULL;
		}
	}
	
	while (pCurrNode != NULL) {
		AnimationEvent* pCurrEvent = (AnimationEvent*)pCurrNode->pData;

		if (fTime < pCurrEvent->getTime())
			return pPrevNode;

		if (bGoForward) {
			pPrevNode = pCurrNode;
			pCurrNode = pCurrNode->pNext;
		}
		else {
			pCurrNode = pPrevNode;
			pPrevNode = pPrevNode->pPrev;
		}
	}
	return pPrevNode;
}

DLLNode* AnimationTimeline::getDLLNodeBeforeCurrent() {
	DLLNode* pAnimEventListHead = m_dllAnimEventList.getHead();
	if (m_pCurrAnimDLLNode == NULL && pAnimEventListHead != NULL) {
		return pAnimEventListHead;
	}

	if (m_pCurrAnimDLLNode->pPrev != NULL)
		return m_pCurrAnimDLLNode->pPrev;
	else
		return NULL;
}

DLLNode* AnimationTimeline::getDLLNodeAfterCurrent() {
	DLLNode* pAnimEventListHead = m_dllAnimEventList.getHead();
	if (m_pCurrAnimDLLNode == NULL && pAnimEventListHead != NULL) {
		return pAnimEventListHead;
	}

	if (m_pCurrAnimDLLNode->pNext != NULL)
		return m_pCurrAnimDLLNode->pNext;
	else
		return NULL;
}

AnimationEvent* AnimationTimeline::getEventBeforeTime(float fTime) {
	DLLNode* pEventDLLNode = getDLLNodeBeforeTime(fTime);
	if (pEventDLLNode != NULL)
		return (AnimationEvent*)pEventDLLNode->pData;
	else
		return NULL;
}

AnimationEvent* AnimationTimeline::getEventAfterCurrent() {
	DLLNode* pAnimEventDLLNode = getDLLNodeAfterCurrent();
	
	if (pAnimEventDLLNode != NULL)
		return (AnimationEvent*)pAnimEventDLLNode->pData;
	else
		return NULL;
}

float AnimationTimeline::getTime() {
	return m_fCurrTime;
}

bool AnimationTimeline::isPlaying() {
	return m_bIsPlaying;
}

void AnimationTimeline::setTime(float fNewTime) {
	m_fCurrTime = fNewTime;
	m_pCurrAnimDLLNode = getDLLNodeBeforeTime(fNewTime, m_pCurrAnimDLLNode);
	if (m_pCurrAnimDLLNode) {
		m_pCurrAnimEvent = (AnimationEvent*)m_pCurrAnimDLLNode->pData;
		printf("%f, %p, %f\n", m_fCurrTime, m_pCurrAnimEvent, m_pCurrAnimEvent->getTime());
		updateEventWidget(m_pCurrAnimDLLNode);
	}
	else {
		m_pCurrAnimEvent = NULL;
	}
}

void AnimationTimeline::setIsPlaying(bool bNewIsPlaying) {
	m_bIsPlaying = bNewIsPlaying;
}

void AnimationTimeline::update() {
	float fDeltaTime = System::get()->getDeltaTime();
	if (m_bIsPlaying) {
		setTime(m_fCurrTime + fDeltaTime * m_fPlaySpeed);
	}
}

void AnimationTimeline::updateEventWidget(DLLNode* pAnimEventDLLNode) {
	AnimationEvent* pCurrAnimEvent = (AnimationEvent*)pAnimEventDLLNode->pData;

	float fTime = pCurrAnimEvent->getTime();
	IWidget* pWidget = pCurrAnimEvent->getWidget();
	EAnimationEventType eEventType = pCurrAnimEvent->getEventType();
	vect2df_t vNewPosition = pCurrAnimEvent->getPosition();

	switch (eEventType) {
	case EAnimationEventType_STEP:
		pWidget->translate(vNewPosition.x, vNewPosition.y, TRANSFORM_ABS);
		break;

	case EAnimationEventType_LINEAR:
		if (pAnimEventDLLNode->pNext != NULL) {
			AnimationEvent* pNextAnimEvent = (AnimationEvent*)pAnimEventDLLNode->pNext->pData;
			
			vect2df_t vNextPosition = pNextAnimEvent->getPosition();

			float fCurrEventTime = pCurrAnimEvent->getTime();
			float fNextEventTime = pNextAnimEvent->getTime();
			float fCurrEventDeltaTime = fNextEventTime - fCurrEventTime;
			float fTimeFramePos = (m_fCurrTime - fCurrEventTime) / fCurrEventDeltaTime;

			pWidget->translate(
				vNewPosition.x + (vNextPosition.x - vNewPosition.x) * fTimeFramePos,
				vNewPosition.y + (vNextPosition.y - vNewPosition.y) * fTimeFramePos,
				TRANSFORM_ABS
			);

		}
		break;
	}
}
