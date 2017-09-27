#ifndef TS_MAINCHARACTER_HPP
#define TS_MAINCHARACTER_HPP

#include "common_types.hpp"
#include "fsm.hpp"
#include "fsm_node.hpp"
#include "image.hpp"
#include "sprite.hpp"
#include "system.hpp"
#include "ts_things_manager.hpp"
#include "ts_draggable_thing.hpp"


enum FSM_MAINCHAR_STATE {
	E_MAINCHAR_STATE_IDLE,
	E_MAINCHAR_STATE_SEEKING,
	E_MAINCHAR_STATE_APPEALED,
	E_MAINCHAR_STATE_WALKING,
	E_MAINCHAR_STATE_OCCUPIED,
};

typedef struct eventTimer_t {
	float currTime;
	float limit;
} eventTimer_t;

typedef struct historyThingInfo_t {
	DraggableThing* pThingObj;
	float fCooldownTimeLeft;
} historyThingInfo_t;


class MainCharacter : public Sprite {

private:
	FSM m_fsm;
	eventTimer_t* m_pEventTimer;
	float* m_pDistToFocusedThing;
	ThingsManager* m_pThingsManager;
	DraggableThing* m_pCurrFocusedThing;
	float m_fWalkSpeed;
	FSM_MAINCHAR_STATE m_prevState;
	LinkedList m_llThingsHistory;

public:
	MainCharacter(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager);
	~MainCharacter();

	void initNewState(FSM_MAINCHAR_STATE currState);

	void update();

	static bool boredEventFunc(void* arg);
	static bool foundThingEventFunc(void* arg);
	static bool startWalkingEventFunc(void* arg);
	static bool startBusyEventFunc(void* arg);
	static bool seenAnotherThingEventFunc(void* arg);
	static bool endBusyEventFunc(void* arg);
};

#endif
