#include "ts_win_character.hpp"
#include "ts_main_character.hpp"


WinCharacter::WinCharacter(SpriteSheet* pSprSh, vect2df_t vPos)
	: AnimatedSprite(pSprSh, vPos)
{
	getClassType()->setClassTypeName("WinCharacter");

	// Creating FSM
	m_pEventTimer = new eventTimer_t;
	m_pEventDist = new eventDist_t;
	m_pEventMainCharState = new eventMainCharState_t;

	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = 5;

	m_eCurrOrientation = E_ORIENTATION_N;

	// Init FSM
	FSMNode* fsmNodeAppear = new FSMNode(E_WINCHAR_STATE_APPEAR, "appear");
	FSMNode* fsmNodeWalking = new FSMNode(E_WINCHAR_STATE_WALKING, "walking");
	FSMNode* fsmNodeWaitMainChar = new FSMNode(E_WINCHAR_STATE_WAIT_MAIN_CHAR, "wait_main_char");
	FSMNode* fsmNodeWait = new FSMNode(E_WINCHAR_STATE_WAIT, "wait");
	FSMNode* fsmNodeWalkback = new FSMNode(E_WINCHAR_STATE_WALKBACK, "walkback");
	FSMNode* fsmNodeDisappear = new FSMNode(E_WINCHAR_STATE_DISAPPEAR, "disappear");
	FSMNode* fsmNodeFinish = new FSMNode(E_WINCHAR_STATE_FINISH, "finished");

	fsmNodeAppear->addEvent("appeared", fsmNodeWalking, &timedEventFunc, &m_pEventTimer);
	fsmNodeWalking->addEvent("arrived_at_point", fsmNodeWaitMainChar, &distToEventFunc, &m_pEventDist);
	fsmNodeWaitMainChar->addEvent("main_char_arrived", fsmNodeWait, &waitForMainCharStateEventFunc, &m_pEventMainCharState);
	fsmNodeWait->addEvent("finish_waiting", fsmNodeWalkback, &timedEventFunc, &m_pEventTimer);
	fsmNodeWalkback->addEvent("arrived_door", fsmNodeDisappear, &distToEventFunc, &m_pEventDist);
	fsmNodeDisappear->addEvent("disappeared", fsmNodeFinish, &timedEventFunc, &m_pEventTimer);

	m_fsm.addNode(fsmNodeAppear);
	m_fsm.addNode(fsmNodeWalking);
	m_fsm.addNode(fsmNodeWait);
	m_fsm.addNode(fsmNodeWalkback);
	m_fsm.addNode(fsmNodeDisappear);

	m_fWalkSpeed = 20.;
    
	addState("idle_s", 0, 5, 10, true);
	addState("walk_n", 6, 13, 10, true);
    addState("walk_s", 14, 21, 10, true);

	updateAnimationState();
}

WinCharacter::~WinCharacter() {

}

void WinCharacter::onNewState(FSM_WINCHAR_STATE currState) {
	switch (currState) {
	case E_WINCHAR_STATE_APPEAR:
		updateAnimationState();
		setEventTimer(5);

		m_eCurrOrientation = E_ORIENTATION_N;

		break;

	case E_WINCHAR_STATE_WALKING:
		updateAnimationState();
		setEventDist(1);

		m_vWalkDest = m_rect.getPos();
		m_vWalkDest.y -= 40;

		break;

	case E_WINCHAR_STATE_WAIT_MAIN_CHAR:
		m_eCurrOrientation = E_ORIENTATION_S;

		updateAnimationState();

		m_pEventMainCharState->pTargetChar = TSGameMode::get()->getFinalMainCharacter();
		m_pEventMainCharState->eState = E_MAINCHAR_STATE_WIN_ARRIVED_AT_CHAR;

		break;

	case E_WINCHAR_STATE_WAIT:
		m_eCurrOrientation = E_ORIENTATION_S;

		updateAnimationState();
		setEventTimer(3);

		break;

	case E_WINCHAR_STATE_WALKBACK:
		updateAnimationState();
		setEventDist(1);

		m_vWalkDest = m_rect.getPos();
		m_vWalkDest.y += 40;

		break;

	case E_WINCHAR_STATE_DISAPPEAR:
		updateAnimationState();
		setEventTimer(1);

		break;

	case E_WINCHAR_STATE_FINISH:
		setActive(false);

		break;
	}
}


void WinCharacter::onEndState(FSM_WINCHAR_STATE currState) {
	
}

void WinCharacter::setEventTimer(int iNewTime) {
	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = iNewTime;
}

void WinCharacter::setEventDist(int iNewDistLimit) {
	m_pEventDist->currDist = 0;
	m_pEventDist->limit = iNewDistLimit;
}

void WinCharacter::update() {
    AnimatedSprite::update();
    
	double deltaTime = System::get()->getDeltaTime();

	m_fsm.update();

	FSM_WINCHAR_STATE currState = (FSM_WINCHAR_STATE) m_fsm.getActiveState()->getStateId();

	if (m_prevState != currState) {
		if (m_prevState != NULL)
			onEndState(m_prevState);

		onNewState(currState);

		m_prevState = currState;
		printf("WinChar: Current state: %s\n", m_fsm.getActiveState()->getName());
	}

	switch (currState) {
		case E_WINCHAR_STATE_WALKBACK:
		case E_WINCHAR_STATE_WALKING:
		{
			walkTo(m_vWalkDest);
			break;
		}
	}
}

void WinCharacter::translate(float x, float y, ETransformMode transformMode) {
    vect2df_t vCurPos = getRect()->getPos();
    
    vect2df_t vDeltaPos;
    
    if (transformMode == TRANSFORM_REL) {
        vDeltaPos.x = x;
        vDeltaPos.y = y;
    }
    else {
        vDeltaPos.x = x - vCurPos.x;
        vDeltaPos.y = y - vCurPos.y;
    }
    
	updateOrientationFromVector(vDeltaPos);
	updateAnimationState();
    
    Sprite::translate(x, y, transformMode);
}

void WinCharacter::walkTo(vect2df_t vDstPos) {
	double deltaTime = System::get()->getDeltaTime();

	vect2df_t vCurrPos = m_rect.getPos();

	vect2df_t vMoveVector;
	vect2df_t vNormalizedVect;

	vMoveVector.x = vCurrPos.x - vDstPos.x;
	vMoveVector.y = vCurrPos.y - vDstPos.y;

	vNormalizedVect = getNormalizedVect(vMoveVector);

	m_pEventDist->currDist = (float)sqrt((vMoveVector.x * vMoveVector.x) + (vMoveVector.y * vMoveVector.y));

	translate((-vNormalizedVect.x * deltaTime * m_fWalkSpeed), (-vNormalizedVect.y * deltaTime * m_fWalkSpeed), TRANSFORM_REL);
}

void WinCharacter::walkTo(IWidget* pDstObj) {
	walkTo(pDstObj->getRect()->getPos());
}

E_ORIENTATION WinCharacter::getOrientationFromVector(vect2df_t vDir) {
	if (vDir.x > 0.) {
		if (vDir.y > 0.) {
			return E_ORIENTATION_SE;
		}
		else if (vDir.y == 0.) {
			return E_ORIENTATION_E;
		}
		else {
			return E_ORIENTATION_NE;
		}
	}
	else if (vDir.x == 0.) {
		if (vDir.y > 0.) {
			return E_ORIENTATION_S;
		}
		else if (vDir.y == 0.) {
			return E_ORIENTATION_UNKNOWN;
		}
		else {
			return E_ORIENTATION_N;
		}
	}
	else if (vDir.x < 0.) {
		if (vDir.y > 0.) {
			return E_ORIENTATION_SO;
		}
		else if (vDir.y == 0.) {
			return E_ORIENTATION_O;
		}
		else {
			return E_ORIENTATION_NO;
		}
	}
}

// Static FSM events funcs
bool WinCharacter::timedEventFunc(void* arg) {
	eventTimer_t* pTimerArg = *((eventTimer_t**) arg);

	if (pTimerArg->currTime > pTimerArg->limit) {
		pTimerArg->currTime = 0;
		return true;
	}

	pTimerArg->currTime += System::get()->getDeltaTime();

	return false;
}

bool WinCharacter::distToEventFunc(void* arg) {
	eventDist_t* pEventDist = (eventDist_t*) *((eventDist_t**)arg);

	if (pEventDist->currDist < pEventDist->limit) {
		return true;
	}

	return false;
}

bool WinCharacter::waitForMainCharStateEventFunc(void* arg) {
	eventMainCharState_t* pEventState = (eventMainCharState_t*) *((eventMainCharState_t**)arg);

	if (pEventState->eState == pEventState->pTargetChar->getCurrentState()) {
		return true;
	}

	return false;
}

void WinCharacter::updateOrientationFromVector(vect2df_t vDeltaPos) {
	E_ORIENTATION eOrientation = getOrientationFromVector(vDeltaPos);

	if (eOrientation != E_ORIENTATION_UNKNOWN)
		m_eCurrOrientation = eOrientation;
}

void WinCharacter::updateAnimationState() {
	switch (m_fsm.getActiveState()->getStateId()) {
		case E_WINCHAR_STATE_WALKING:
		case E_WINCHAR_STATE_WALKBACK:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_N:
				changeState(1);
				break;

			case E_ORIENTATION_S:
				changeState(2);
				break;
			}
			break;
		}

		case E_WINCHAR_STATE_APPEAR:
		case E_WINCHAR_STATE_WAIT:
		case E_WINCHAR_STATE_WAIT_MAIN_CHAR:
		case E_WINCHAR_STATE_DISAPPEAR:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_S:
				changeState(0);
				break;
			}
			break;
		}
	}
}