#include "ts_main_character.hpp"
#include "math_tools.hpp"


MainCharacter::MainCharacter(SpriteSheet* pSprSh, vect2df_t vPos, ThingsManager* pThingsManager)
	: AnimatedSprite(pSprSh, vPos)
{
	// Creating FSM
	m_pEventTimer = new eventTimer_t;
	m_pDistToFocusedThing = new float(0);

	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = 5;

	m_pCurrentJob = NULL;

	// Init progress bar
	vect2df_t vProgressBarPos;
	vProgressBarPos.x = m_rect.getPos().x;
	vProgressBarPos.y = m_rect.getPos().y - 10;

	size2df_t sProgressBarSize;
	sProgressBarSize.w = 20;
	sProgressBarSize.h = 3;

	m_pProgressBar = new TinyProgressBar(vProgressBarPos, sProgressBarSize);
	m_pProgressBar->setActive(false);

	m_eCurrOrientation = E_ORIENTATION_S;

	// Init FSM
	FSMNode* fsmNodeIdle = new FSMNode(E_MAINCHAR_STATE_IDLE, "idle");
	FSMNode* fsmNodeSeeking = new FSMNode(E_MAINCHAR_STATE_SEEKING, "seeking");
	FSMNode* fsmNodeAppealed = new FSMNode(E_MAINCHAR_STATE_APPEALED, "appealed");
	FSMNode* fsmNodeWalking = new FSMNode(E_MAINCHAR_STATE_WALKING, "walking");
	FSMNode* fsmNodeArrived = new FSMNode(E_MAINCHAR_STATE_ARRIVED, "arrived");
	FSMNode* fsmNodeOccupiedObj = new FSMNode(E_MAINCHAR_STATE_OCCUPIED_OBJ, "occupied_object");
	FSMNode* fsmNodeOccupiedCrit = new FSMNode(E_MAINCHAR_STATE_OCCUPIED_CRIT, "occupied_critical");
	FSMNode* fsmNodeOccupiedWorkguy = new FSMNode(E_MAINCHAR_STATE_OCCUPIED_WORKGUY, "occupied_workguy");

	fsmNodeIdle->addEvent("bored", fsmNodeSeeking, &boredEventFunc, &m_pEventTimer);
	fsmNodeSeeking->addEvent("found", fsmNodeAppealed, &foundThingEventFunc, &m_pCurrFocusedThing);
	fsmNodeAppealed->addEvent("start_walking", fsmNodeWalking, &startWalkingEventFunc, &m_pEventTimer);
	fsmNodeWalking->addEvent("arrived_on_thing", fsmNodeArrived, &startBusyEventFunc, &m_pDistToFocusedThing);
	fsmNodeWalking->addEvent("start_walking_to_another", fsmNodeAppealed, &seenAnotherThingEventFunc, &m_pEventTimer);
	fsmNodeArrived->addEvent("thing_is_an_object", fsmNodeOccupiedObj, &determineThingIsObjFuncEvent, &m_pCurrFocusedThing);
	fsmNodeArrived->addEvent("thing_is_a_work_guy", fsmNodeOccupiedWorkguy, &determineThingIsWorkguyFuncEvent, &m_pCurrFocusedThing);
	fsmNodeArrived->addEvent("thing_is_a_critical_object", fsmNodeOccupiedCrit, &determineThingIsCritFuncEvent, &m_pCurrFocusedThing);
	fsmNodeOccupiedObj->addEvent("end_busy", fsmNodeIdle, &endBusyEventFunc, &m_pEventTimer);
	fsmNodeOccupiedCrit->addEvent("end_crit", fsmNodeAppealed, &endAttackEventFunc, &m_pNewFocusedThing);
	fsmNodeOccupiedWorkguy->addEvent("end_workguy", fsmNodeIdle, &endBusyEventFunc, &m_pEventTimer);

	m_fsm.addNode(fsmNodeIdle);
	m_fsm.addNode(fsmNodeSeeking);
	m_fsm.addNode(fsmNodeAppealed);
	m_fsm.addNode(fsmNodeWalking);
	m_fsm.addNode(fsmNodeOccupiedObj);
	m_fsm.addNode(fsmNodeOccupiedCrit);

	m_pThingsManager = pThingsManager;

	m_pCurrFocusedThing = NULL;

	m_fWalkSpeed = 20.;

	initList(&m_llThingsHistory);
    
    addState("walk_n", 0, 7, 10, true);
    addState("walk_ne", 8, 15, 10, true);
    addState("walk_e", 16, 23, 10, true);
    addState("walk_se", 24, 31, 10, true);
    addState("walk_s", 32, 39, 10, true);
    addState("walk_so", 40, 47, 10, true);
    addState("walk_o", 48, 55, 10, true);
    addState("walk_no", 56, 63, 10, true);
    addState("walk_amazed1_e", 64, 71, 10, true);
    addState("walk_amazed2_e", 72, 79, 10, true);
    addState("walk_amazed1_se", 80, 87, 10, true);
    addState("walk_amazed2_se", 88, 95, 10, true);
    addState("walk_amazed1_s", 96, 103, 10, true);
    addState("walk_amazed2_s", 104, 111, 10, true);
    addState("occupied", 112, 115, 5, true);
    addState("idle_n", 116, 116, 5, true);
    addState("idle_ne", 117, 117, 1, true);
    addState("idle_e", 118, 121, 4, true);
	addState("idle_se", 122, 125, 4, true);
	addState("idle_s", 126, 130, 4, true);
	addState("idle_so", 131, 134, 4, true);
	addState("idle_o", 135, 138, 4, true);
	addState("idle_no", 139, 139, 4, true);

	updateAnimationState();
}

MainCharacter::~MainCharacter() {
	if (m_pCurrentJob != NULL) {
		delete m_pCurrentJob;
	}
}

void MainCharacter::onNewState(FSM_MAINCHAR_STATE currState) {
	switch (currState) {
	case E_MAINCHAR_STATE_IDLE:
		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = 5;
		m_pCurrFocusedThing = NULL;

		updateAnimationState();

		break;

	case E_MAINCHAR_STATE_SEEKING:
		m_pCurrFocusedThing = NULL;

		updateAnimationState();

		break;

	case E_MAINCHAR_STATE_OCCUPIED_OBJ:
	case E_MAINCHAR_STATE_OCCUPIED_WORKGUY:
	{
        changeState(14);
        
		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = m_pCurrFocusedThing->getOccupationTime();

		historyThingInfo_t* pHistoryThingInfo = new historyThingInfo_t; // NE PAS OUBLIER DE LE DELETE
		pHistoryThingInfo->pThingObj = m_pCurrFocusedThing;
		pHistoryThingInfo->fCooldownTimeLeft = m_pCurrFocusedThing->getCooldownTime();

		addDataToList(&m_llThingsHistory, pHistoryThingInfo);

		onBeginUsing();
		m_pCurrFocusedThing->onBeginUsing();
		break;
	}

	case E_MAINCHAR_STATE_OCCUPIED_CRIT:
		m_pNewFocusedThing = NULL;
		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = 1;
		onBeginAttack();
		break;
	}
}

void MainCharacter::onEndState(FSM_MAINCHAR_STATE currState) {
	switch (currState) {
	case E_MAINCHAR_STATE_OCCUPIED_OBJ:
	case E_MAINCHAR_STATE_OCCUPIED_WORKGUY:
		onEndUsing();
		m_pCurrFocusedThing->onEndUsing(this);
		break;

	case E_MAINCHAR_STATE_OCCUPIED_CRIT:
		onEndAttack();
		m_pCurrFocusedThing = m_pNewFocusedThing;
		break;
	}
}

void MainCharacter::draw(uint8* fb) {
	AnimatedSprite::draw(fb);

	if (m_pProgressBar->isActive())
		m_pProgressBar->draw(fb);
}

void MainCharacter::update() {
    AnimatedSprite::update();
    
	double deltaTime = System::get()->getDeltaTime();

	m_fsm.update();

	FSM_MAINCHAR_STATE currState = (FSM_MAINCHAR_STATE) m_fsm.getActiveState()->getStateId();

	if (m_prevState != currState) {
		if (m_prevState != NULL)
			onEndState(m_prevState);

		onNewState(currState);

		m_prevState = currState;
		printf("Current state: %s\n", m_fsm.getActiveState()->getName());
	}

	switch (currState) {
		case E_MAINCHAR_STATE_SEEKING:
		{
			m_pCurrFocusedThing = searchForAvailableThings();
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

			vNormalizedVect = getNormalizedVect(vMoveVector);

			//printf("(%f, %f) ", vNormalizedVect.x, vNormalizedVect.y);

			*m_pDistToFocusedThing = (float) sqrt((vMoveVector.x * vMoveVector.x) + (vMoveVector.y * vMoveVector.y));

			translate((-vNormalizedVect.x * deltaTime * m_fWalkSpeed), (-vNormalizedVect.y * deltaTime * m_fWalkSpeed), TRANSFORM_REL);

			break;
		}

		case E_MAINCHAR_STATE_OCCUPIED_OBJ:
		case E_MAINCHAR_STATE_OCCUPIED_WORKGUY:
			onUsing(m_pEventTimer->currTime);
			m_pCurrFocusedThing->onUsing();
			break;

		case E_MAINCHAR_STATE_OCCUPIED_CRIT:
		{
			DraggableThing* newFocusedThing = searchForAvailableThings();
			if (newFocusedThing->getClassType()->getClassTypeName() != "CriticalThing") {
				m_pNewFocusedThing = newFocusedThing;
			}
			onAttacking();
			break;
		}
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

void MainCharacter::translate(float x, float y, ETransformMode transformMode) {
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

E_ORIENTATION MainCharacter::getOrientationFromVector(vect2df_t vDir) {
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

bool MainCharacter::hasWork() {
	return m_bHasWork;
}

void MainCharacter::setFocusedThing(DraggableThing* pThing) {
	m_pCurrFocusedThing = pThing;
}

void MainCharacter::setHasWork(bool bHasWork) {
	m_bHasWork = bHasWork;
}

// Event funcs
void MainCharacter::onBeginUsing() {
	m_pProgressBar->setActive(true);
	m_pProgressBar->setValue(0.);
}

void MainCharacter::onEndUsing() {
	m_pProgressBar->setActive(false);
}

void MainCharacter::onUsing(float fUsageTimeLeft) {
	m_pProgressBar->getRect()->setPos(m_rect.getPos().x, m_rect.getPos().y - 10);
	m_pProgressBar->setValue(fUsageTimeLeft / (float) m_pCurrFocusedThing->getOccupationTime());
}

void MainCharacter::onBeginAttack() {

}

void MainCharacter::onEndAttack() {

}

void MainCharacter::onAttacking() {
	if (m_pEventTimer->currTime > m_pEventTimer->limit) {
		m_pEventTimer->currTime = 0;
		onSlay();
	}
	else {
		m_pEventTimer->currTime += System::get()->getDeltaTime();
	}
}

void MainCharacter::onSlay() {
	TSGameMode::get()->decreaseHealth(1);
	printf("SLAYED\n");
}


void MainCharacter::onEndWork() {
	setHasWork(false);

	TSGameMode::get()->increaseMoney(m_pCurrentJob->m_iPrice);

	delete m_pCurrentJob;
	m_pCurrentJob = NULL;
}

void MainCharacter::onThingMoved() {
	if (m_pCurrFocusedThing != NULL) {
		FSM_MAINCHAR_STATE currState = (FSM_MAINCHAR_STATE) m_fsm.getActiveState()->getStateId();

		if (currState == E_MAINCHAR_STATE_WALKING) {
			m_pCurrFocusedThing = NULL;
			m_fsm.changeState(E_MAINCHAR_STATE_SEEKING);
		}
	}
}


void MainCharacter::assignNewJob(WorkguyThing* pEmployer, int iPrice) {
	setHasWork(true);

	m_pCurrentJob = new work_job_t;
	m_pCurrentJob->m_pEmployer = pEmployer;
	m_pCurrentJob->m_iPrice = iPrice;
	m_pCurrentJob->m_fTimeStarted = System::get()->getTime();
}


DraggableThing* MainCharacter::searchForAvailableThings() {
	LinkedList llSortedAvailableThings;
	initList(&llSortedAvailableThings);

	m_pThingsManager->getClosestAvailableThingsToPoint(&llSortedAvailableThings, m_rect.getPosi(), this);

	DraggableThing* pFoundThing = NULL;

	LLNode* currAvailThingsNode = llSortedAvailableThings.pHead;

	//printf("llSortedAvailableThings: %i\n", (int) llSortedAvailableThings.size);

	while (currAvailThingsNode != NULL) {
		DraggableThing* pCurrThingCandidate = (DraggableThing*)currAvailThingsNode->pData;
		//printf("Current candidate thing : %p\n", pCurrThingCandidate);

		if (pCurrThingCandidate != m_pCurrFocusedThing) {
			LLNode* currHistoryThingsNode = m_llThingsHistory.pHead;

			bool bThingFoundInHistory = false;

			while (currHistoryThingsNode != NULL) {
				historyThingInfo_t* currHistoryNodeInfo = (historyThingInfo_t*)currHistoryThingsNode->pData;

				//printf("%p : %p\n", currAvailThingsNode->pData, currHistoryNodeInfo->pThingObj);

				if (pCurrThingCandidate == currHistoryNodeInfo->pThingObj) {
					bThingFoundInHistory = true;
					break;
				}
				currHistoryThingsNode = currHistoryThingsNode->pNext;
			}

			if (!bThingFoundInHistory) {
				pFoundThing = pCurrThingCandidate;
				//printf("Thing selected: %p\n", pCurrThingCandidate);
				break;
			}
		}

		currAvailThingsNode = currAvailThingsNode->pNext;
	}

	destroyList(&llSortedAvailableThings);

	return pFoundThing;
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
	eventTimer_t* pTimerArg = *((eventTimer_t**)arg);

	if (pTimerArg->currTime > pTimerArg->limit) {
		pTimerArg->currTime = 0;
		return true;
	}

	pTimerArg->currTime += System::get()->getDeltaTime();

	return false;
}

bool MainCharacter::endAttackEventFunc(void* arg) {
	DraggableThing* pNewThing = *((DraggableThing**) arg);

	if (pNewThing != NULL) {
		return true;
	}

	return false;
}

bool MainCharacter::determineThingIsObjFuncEvent(void* arg) {
	DraggableThing* pThing = *((DraggableThing**)arg);
	if (pThing->getClassType()->getClassTypeName() == "DraggableThing") {
		return true;
	}
	return false;
}

bool MainCharacter::determineThingIsCritFuncEvent(void* arg) {
	DraggableThing* pThing = *((DraggableThing**)arg);
	if (pThing->getClassType()->getClassTypeName() == "CriticalThing") {
		return true;
	}
	return false;
}

bool MainCharacter::determineThingIsWorkguyFuncEvent(void* arg) {
	DraggableThing* pThing = *((DraggableThing**)arg);
	if (pThing->getClassType()->getClassTypeName() == "WorkguyThing") {
		return true;
	}
	return false;
}

void MainCharacter::updateOrientationFromVector(vect2df_t vDeltaPos) {
	E_ORIENTATION eOrientation = getOrientationFromVector(vDeltaPos);

	if (eOrientation != E_ORIENTATION_UNKNOWN)
		m_eCurrOrientation = eOrientation;
}

void MainCharacter::updateAnimationState() {
	switch (m_fsm.getActiveState()->getStateId()) {
		case E_MAINCHAR_STATE_WALKING:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_N:
				changeState(0);
				break;

			case E_ORIENTATION_NE:
				changeState(1);
				break;

			case E_ORIENTATION_E:
				changeState(2);
				break;

			case E_ORIENTATION_SE:
				changeState(3);
				break;

			case E_ORIENTATION_S:
				changeState(4);
				break;

			case E_ORIENTATION_SO:
				changeState(5);
				break;

			case E_ORIENTATION_O:
				changeState(6);
				break;

			case E_ORIENTATION_NO:
				changeState(7);
				break;
			}
			break;
		}

		case E_MAINCHAR_STATE_IDLE:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_N:
				changeState(15);
				break;

			case E_ORIENTATION_NE:
				changeState(16);
				break;

			case E_ORIENTATION_E:
				changeState(17);
				break;

			case E_ORIENTATION_SE:
				changeState(18);
				break;

			case E_ORIENTATION_S:
				changeState(19);
				break;

			case E_ORIENTATION_SO:
				changeState(20);
				break;

			case E_ORIENTATION_O:
				changeState(21);
				break;

			case E_ORIENTATION_NO:
				changeState(22);
				break;
			}
			break;
		}
	}
}