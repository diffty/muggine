#ifndef TS_MAINCHARACTER_HPP
#define TS_MAINCHARACTER_HPP

#include <cstring>
#include "common_types.hpp"
#include "fsm.hpp"
#include "fsm_node.hpp"
#include "image.hpp"
#include "animatedsprite.hpp"
#include "system.hpp"
#include "ts_things_manager.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_critical_thing.hpp"


enum FSM_MAINCHAR_STATE {
	E_MAINCHAR_STATE_IDLE,
	E_MAINCHAR_STATE_SEEKING,
	E_MAINCHAR_STATE_APPEALED,
	E_MAINCHAR_STATE_WALKING,
	E_MAINCHAR_STATE_ARRIVED,
	E_MAINCHAR_STATE_OCCUPIED_OBJ,
	E_MAINCHAR_STATE_OCCUPIED_CRIT,
	E_MAINCHAR_STATE_OCCUPIED_WORKGUY,
};

typedef struct eventTimer_t {
	float currTime;
	float limit;
} eventTimer_t;

typedef struct historyThingInfo_t {
	DraggableThing* pThingObj;
	float fCooldownTimeLeft;
} historyThingInfo_t;


class MainCharacter : public AnimatedSprite {

private:
	FSM m_fsm;
	eventTimer_t* m_pEventTimer;
	float* m_pDistToFocusedThing;
	ThingsManager* m_pThingsManager;
	DraggableThing* m_pCurrFocusedThing;
	DraggableThing* m_pNewFocusedThing;
	float m_fWalkSpeed;
	FSM_MAINCHAR_STATE m_prevState;
	LinkedList m_llThingsHistory;
	TinyProgressBar* m_pProgressBar;

public:
	MainCharacter(SpriteSheet* pSprSh, vect2df_t vPos, ThingsManager* pThingsManager);
	~MainCharacter();

	void onNewState(FSM_MAINCHAR_STATE currState);
	void onEndState(FSM_MAINCHAR_STATE currState);

	void draw(uint8* fb);
	void update();
    void translate(float x, float y, ETransformMode transformMode);
    
	void onBeginUsing();
	void onEndUsing();
	void onUsing(float fCurUsageTime);

	void onBeginAttack();
	void onEndAttack();
	void onAttacking();
	void onSlay();

	void onThingMoved();

	DraggableThing* searchForAvailableThings();

	static bool boredEventFunc(void* arg);
	static bool foundThingEventFunc(void* arg);
	static bool startWalkingEventFunc(void* arg);
	static bool startBusyEventFunc(void* arg);
	static bool seenAnotherThingEventFunc(void* arg);
	static bool endBusyEventFunc(void* arg);
	static bool endAttackEventFunc(void* arg);
	static bool determineThingIsObjFuncEvent(void* arg);
	static bool determineThingIsCritFuncEvent(void* arg);
};

#endif
