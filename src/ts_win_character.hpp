#ifndef TS_WINCHARACTER_HPP
#define TS_WINCHARACTER_HPP

#include <cstring>
#include "common_types.hpp"
#include "mathf.hpp"
#include "fsm.hpp"
#include "fsm_node.hpp"
#include "image.hpp"
#include "animatedsprite.hpp"
#include "system.hpp"
#include "ts_common_types.hpp"
#include "ts_game_mode.hpp"



class WinCharacter : public AnimatedSprite {

private:
	FSM m_fsm;

	eventTimer_t* m_pEventTimer;
	eventDist_t* m_pEventDist;
	eventMainCharState_t* m_pEventMainCharState;

	float m_fWalkSpeed;
	vect2df_t m_vWalkDest;

	E_ORIENTATION m_eCurrOrientation;
	FSM_WINCHAR_STATE m_prevState;

public:
	WinCharacter(SpriteSheet* pSprSh, vect2df_t vPos);
	~WinCharacter();

	void onNewState(FSM_WINCHAR_STATE currState);
	void onEndState(FSM_WINCHAR_STATE currState);

	void setEventTimer(int iNewTime);
	void setEventDist(int iNewDistLimit);

	void update();
    void translate(float x, float y, ETransformMode transformMode);

	void walkTo(IWidget* pDstObj);
	void walkTo(vect2df_t vDstPos);

	E_ORIENTATION getOrientationFromVector(vect2df_t vDeltaPos);

	static bool timedEventFunc(void* arg);
	static bool distToEventFunc(void* arg);
	static bool waitForMainCharStateEventFunc(void* arg);

	void updateOrientationFromVector(vect2df_t vDeltaPos);
	void updateAnimationState();
};

#endif
