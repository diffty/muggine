#ifndef TS_MAINCHARACTER_HPP
#define TS_MAINCHARACTER_HPP

#include <cstring>
#include "common_types.hpp"
#include "mathf.hpp"
#include "fsm.hpp"
#include "fsm_node.hpp"
#include "image.hpp"
#include "animatedsprite.hpp"
#include "system.hpp"
#include "ts_things_manager.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_critical_thing.hpp"
#include "ts_workguy_thing.hpp"
#include "ts_common_types.hpp"
#include "ts_text_bubble.hpp"


typedef struct historyThingInfo_t {
	DraggableThing* pThingObj;
	float fCooldownTimeLeft;
} historyThingInfo_t;


class MainCharacter : public AnimatedSprite {

private:
	FSM m_fsm;
	eventTimer_t* m_pEventTimer;
	eventDist_t* m_pEventDist;
	ThingsManager* m_pThingsManager;
	DraggableThing* m_pCurrFocusedThing;
	DraggableThing* m_pNewFocusedThing;
	float m_fWalkSpeed;
	LinkedList m_llThingsHistory;
	TinyProgressBar* m_pProgressBar;
	E_ORIENTATION m_eCurrOrientation;
	FSM_MAINCHAR_STATE m_prevState;
	bool m_bHasWork;
	work_job_t* m_pCurrentJob;
	TextBubble* m_pTextBubble;
	float m_fBubbleTextTimeLeft;

public:
	MainCharacter(SpriteSheet* pSprSh, vect2df_t vPos, ThingsManager* pThingsManager);
	~MainCharacter();

	void onNewState(FSM_MAINCHAR_STATE currState);
	void onEndState(FSM_MAINCHAR_STATE currState);

	void draw(uint8* buffer);
	void update();
    void translate(float x, float y, ETransformMode transformMode);

	void walkTo(IWidget* pDstObj);
	void walkTo(vect2df_t vDstPos);

	E_ORIENTATION getOrientationFromVector(vect2df_t vDeltaPos);
	FSM_MAINCHAR_STATE getCurrentState();

	bool hasWork();
	void setFocusedThing(DraggableThing* pThing);
	void setHasWork(bool bHasWork);
	void setEventTimer(int iNewTime);
	void setEventDist(int iNewDistLimit);
    
	void showBubble(char* szStr);

	void onBeginUsing();
	void onEndUsing();
	void onUsing(float fCurUsageTime);

	void onBeginAttack();
	void onEndAttack();
	void onAttacking();
	void onSlay();

	void onEndWork();
	void onThingMoved();
	void onWinWalk();
	void onWinVanish();

	void assignNewJob(WorkguyThing* pEmployer, int iPrice);

	DraggableThing* searchForAvailableThings();

	static bool timedEventFunc(void* arg);
	static bool distToEventFunc(void* arg);
	static bool validThingEventFunc(void* arg);
	static bool startWalkingEventFunc(void* arg);
	static bool seenAnotherThingEventFunc(void* arg);
	static bool determineThingIsObjFuncEvent(void* arg);
	static bool determineThingIsCritFuncEvent(void* arg);
	static bool determineThingIsWorkguyFuncEvent(void* arg);

	void updateOrientationFromVector(vect2df_t vDeltaPos);
	void updateAnimationState();
};

#endif
