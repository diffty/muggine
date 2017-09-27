#include "ts_main_character.hpp"

bool test(void* pCbArg) {
	int* testVar = (int*) *((int**)pCbArg);

	if (*testVar > 5) {
		*testVar = 0;
		return true;
	}
	return false;
}

bool test2(void* pCbArg) {
	int* testVar = (int*) *((int**)pCbArg);

	if (*testVar > 5) {
		return true;
	}
	return false;
}



MainCharacter::MainCharacter(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager)
	: Sprite(pImg, vPos)
{
	// Creating FSM
	m_pEventTimer = new eventTimer_t;
	m_pDistToFocusedThing = new float(0);

	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = 5;

	FSMNode* fsmNodeIdle = new FSMNode(E_MAINCHAR_STATE_IDLE, "idle");
	FSMNode* fsmNodeSeeking = new FSMNode(E_MAINCHAR_STATE_SEEKING, "seeking");
	FSMNode* fsmNodeAppealed = new FSMNode(E_MAINCHAR_STATE_APPEALED, "appealed");
	FSMNode* fsmNodeWalking = new FSMNode(E_MAINCHAR_STATE_WALKING, "walking");
	FSMNode* fsmNodeOccupied = new FSMNode(E_MAINCHAR_STATE_OCCUPIED, "occupied");

	fsmNodeIdle->addEvent("bored", fsmNodeSeeking, &boredEventFunc, &m_pEventTimer);
	fsmNodeSeeking->addEvent("found", fsmNodeAppealed, &foundThingEventFunc, &m_pCurrFocusedThing);
	fsmNodeAppealed->addEvent("start_walking", fsmNodeWalking, &startWalkingEventFunc, &m_pEventTimer);
	fsmNodeWalking->addEvent("arrived_on_thing", fsmNodeOccupied, &startBusyEventFunc, &m_pDistToFocusedThing);
	fsmNodeWalking->addEvent("start_walking_to_another", fsmNodeAppealed, &seenAnotherThingEventFunc, &m_pEventTimer);
	fsmNodeOccupied->addEvent("end_busy", fsmNodeIdle, &endBusyEventFunc, &m_pEventTimer);

	m_fsm.addNode(fsmNodeIdle);
	m_fsm.addNode(fsmNodeSeeking);
	m_fsm.addNode(fsmNodeAppealed);
	m_fsm.addNode(fsmNodeWalking);
	m_fsm.addNode(fsmNodeOccupied);

	m_pThingsManager = pThingsManager;

	m_pCurrFocusedThing = NULL;

	m_fWalkSpeed = 20.;

	initList(&m_llThingsHistory);
}

MainCharacter::~MainCharacter() {

}

void MainCharacter::initNewState(FSM_MAINCHAR_STATE currState) {
	switch (currState) {
	case E_MAINCHAR_STATE_IDLE:
		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = 5;
		m_pCurrFocusedThing = NULL;
		break;

	case E_MAINCHAR_STATE_OCCUPIED:
		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = 5;

		historyThingInfo_t* pHistoryThingInfo = new historyThingInfo_t; // NE PAS OUBLIER DE LE DELETE
		pHistoryThingInfo->pThingObj = m_pCurrFocusedThing;
		pHistoryThingInfo->fCooldownTimeLeft = 10.;

		addDataToList(&m_llThingsHistory, pHistoryThingInfo);
		break;
	}
}

void MainCharacter::update() {
	double deltaTime = System::get()->getDeltaTime();

	m_fsm.update();

	FSM_MAINCHAR_STATE currState = (FSM_MAINCHAR_STATE) m_fsm.getActiveState()->getStateId();

	if (m_prevState != currState) {
		initNewState(currState);
		m_prevState = currState;
		printf("Current state: %s\n", m_fsm.getActiveState()->getName());
	}

	switch (currState) {
		case E_MAINCHAR_STATE_SEEKING:
		{
			LinkedList llSortedAvailableThings;
			initList(&llSortedAvailableThings);

			m_pThingsManager->getClosestAvailableThingsToPoint(&llSortedAvailableThings, m_rect.getPosi());

			m_pCurrFocusedThing = NULL;

			LLNode* currAvailThingsNode = llSortedAvailableThings.pHead;

			printf("llSortedAvailableThings: %i\n", llSortedAvailableThings.size);

			while (currAvailThingsNode != NULL) {
				DraggableThing* pCurrThingCandidate = (DraggableThing*) currAvailThingsNode->pData;

				printf("%p\n", pCurrThingCandidate);
				LLNode* currHistoryThingsNode = m_llThingsHistory.pHead;

				bool bThingFound = false;

				while (currHistoryThingsNode != NULL) {
					historyThingInfo_t* currHistoryNodeInfo = (historyThingInfo_t*) currHistoryThingsNode->pData;

					printf("%p : %p\n", currAvailThingsNode->pData, currHistoryNodeInfo->pThingObj);

					if (pCurrThingCandidate == currHistoryNodeInfo->pThingObj) {
						bThingFound = true;
						break;
					}
					currHistoryThingsNode = currHistoryThingsNode->pNext;
				}

				if (!bThingFound) {
					m_pCurrFocusedThing = pCurrThingCandidate;
					printf("Thing selected: %p\n", pCurrThingCandidate);
					break;
				}

				currAvailThingsNode = currAvailThingsNode->pNext;
			}

			destroyList(&llSortedAvailableThings);

			break;
		}

		case E_MAINCHAR_STATE_WALKING:
		{
			vect2df_t vCurrFocusedThingPos = m_pCurrFocusedThing->getRect()->getPos();
			vect2df_t vCurrPos = m_rect.getPos();

			vect2df_t vMoveVector;
			vect2df_t vNormalizedVect;

			vMoveVector.x = vCurrPos.x - vCurrFocusedThingPos.x;
			vMoveVector.y = vCurrPos.y - vCurrFocusedThingPos.y;

			if (abs(vMoveVector.x) < abs(vMoveVector.y)) {
				vNormalizedVect.x = vMoveVector.x / abs(vMoveVector.y);
				vNormalizedVect.y = vMoveVector.y / abs(vMoveVector.y);
			}
			else if (abs(vMoveVector.x) > abs(vMoveVector.y)) {
				vNormalizedVect.x = vMoveVector.x / abs(vMoveVector.x);
				vNormalizedVect.y = vMoveVector.y / abs(vMoveVector.x);
			}
			else {
				vNormalizedVect.x = vMoveVector.x / abs(vMoveVector.x);
				vNormalizedVect.y = vMoveVector.y / abs(vMoveVector.y);
			}

			//printf("(%f, %f) ", vNormalizedVect.x, vNormalizedVect.y);

			*m_pDistToFocusedThing = (float) sqrt((vMoveVector.x * vMoveVector.x) + (vMoveVector.y * vMoveVector.y));

			translate((-vNormalizedVect.x * deltaTime * m_fWalkSpeed), (-vNormalizedVect.y * deltaTime * m_fWalkSpeed), TRANSFORM_REL);

			break;
		}

		case E_MAINCHAR_STATE_OCCUPIED:
			break;
	}

	// Updating things cooldown countdown
	LLNode* currHistoryThingsNode = m_llThingsHistory.pHead;
	LLNode* nextHistoryThingsNode = NULL;

	while (currHistoryThingsNode != NULL) {
		historyThingInfo_t* currHistoryNodeInfo = (historyThingInfo_t*)currHistoryThingsNode->pData;

		nextHistoryThingsNode = currHistoryThingsNode->pNext;

		if (currHistoryNodeInfo->fCooldownTimeLeft != -1
			&& currHistoryNodeInfo->pThingObj != m_pCurrFocusedThing) {
			currHistoryNodeInfo->fCooldownTimeLeft -= System::get()->getDeltaTime();
			if (currHistoryNodeInfo->fCooldownTimeLeft < 0.) {
				removeNodeFromList(&m_llThingsHistory, currHistoryThingsNode);
				delete currHistoryNodeInfo;
				delete currHistoryThingsNode;
			}
		}
		currHistoryThingsNode = nextHistoryThingsNode;
	}
}

// Static FSM events funcs
bool MainCharacter::boredEventFunc(void* arg) {
	eventTimer_t* pTimerArg = *((eventTimer_t**) arg);

	if (pTimerArg->currTime > pTimerArg->limit) {
		pTimerArg->currTime = 0;
		return true;
	}

	pTimerArg->currTime += System::get()->getDeltaTime();

	return false;
}

bool MainCharacter::foundThingEventFunc(void* arg) {
	DraggableThing* pCurrThing = *((DraggableThing**) arg);
	if (pCurrThing) {
		return true;
	}
	return false;
}

bool MainCharacter::startWalkingEventFunc(void* arg) {
	return true;
}

bool MainCharacter::startBusyEventFunc(void* arg) {
	float* fDistToFocusedThing = (float*) *((float**) arg);

	if (*fDistToFocusedThing < 5) {
		return true;
	}

	return false;
}

bool MainCharacter::seenAnotherThingEventFunc(void* arg) {
	return false;
}

bool MainCharacter::endBusyEventFunc(void* arg) {
	eventTimer_t* pTimerArg = *((eventTimer_t**) arg);

	if (pTimerArg->currTime > pTimerArg->limit) {
		pTimerArg->currTime = 0;
		return true;
	}

	pTimerArg->currTime += System::get()->getDeltaTime();

	return false;
}
