#include "ts_main_character.hpp"
#include "ts_win_character.hpp"


#define BUBBLE_STAY_TIME 3


MainCharacter::MainCharacter(SpriteSheet* pSprSh, vect2df_t vPos, ThingsManager* pThingsManager)
	: AnimatedSprite(pSprSh, vPos),
	m_speechData("data/main_char_text.csv")
{
	getClassType()->setClassTypeName("MainCharacter");
	
	// Creating FSM
	m_pEventTimer = new eventTimer_t;
	m_pEventDist = new eventDist_t;

	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = 5;

	m_pEventDist->currDist = 0;
	m_pEventDist->limit = 5;

	m_pCurrentJob = NULL;

	m_fBubbleTextTimeLeft = -1;

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
	FSMNode* fsmNodeWinAppealed = new FSMNode(E_MAINCHAR_STATE_WIN_APPEALED, "win_appealed");
	FSMNode* fsmNodeWinWalkingToChar = new FSMNode(E_MAINCHAR_STATE_WIN_WALKING_TO_CHAR, "win_walking_to_char");
	FSMNode* fsmNodeWinArrivedAtChar = new FSMNode(E_MAINCHAR_STATE_WIN_ARRIVED_AT_CHAR, "win_arrived_at_win_char");
	FSMNode* fsmNodeWinWalkingToExit = new FSMNode(E_MAINCHAR_STATE_WIN_WALKING_TO_EXIT, "win_walking_to_exit");
	FSMNode* fsmNodeWinVanish = new FSMNode(E_MAINCHAR_STATE_WIN_VANISH, "win_vanish");
	FSMNode* fsmNodeWinOut = new FSMNode(E_MAINCHAR_STATE_WIN_OUT, "win_out");

	fsmNodeIdle->addEvent("bored", fsmNodeSeeking, &timedEventFunc, &m_pEventTimer);
	fsmNodeSeeking->addEvent("found", fsmNodeAppealed, &validThingEventFunc, &m_pCurrFocusedThing);
	fsmNodeAppealed->addEvent("start_walking", fsmNodeWalking, &timedEventFunc, &m_pEventTimer);
	fsmNodeWalking->addEvent("arrived_on_thing", fsmNodeArrived, &distToEventFunc, &m_pEventDist);
	fsmNodeWalking->addEvent("start_walking_to_another", fsmNodeAppealed, &seenAnotherThingEventFunc, &m_pEventTimer);
	fsmNodeArrived->addEvent("thing_is_an_object", fsmNodeOccupiedObj, &determineThingIsObjFuncEvent, &m_pCurrFocusedThing);
	fsmNodeArrived->addEvent("thing_is_a_work_guy", fsmNodeOccupiedWorkguy, &determineThingIsWorkguyFuncEvent, &m_pCurrFocusedThing);
	fsmNodeArrived->addEvent("thing_is_a_critical_object", fsmNodeOccupiedCrit, &determineThingIsCritFuncEvent, &m_pCurrFocusedThing);
	fsmNodeOccupiedObj->addEvent("end_busy", fsmNodeIdle, &timedEventFunc, &m_pEventTimer);
	fsmNodeOccupiedCrit->addEvent("end_crit", fsmNodeAppealed, &validThingEventFunc, &m_pNewFocusedThing);
	fsmNodeOccupiedWorkguy->addEvent("end_workguy", fsmNodeIdle, &timedEventFunc, &m_pEventTimer);

	fsmNodeWinAppealed->addEvent("win_appealed", fsmNodeWinWalkingToChar, &timedEventFunc, &m_pEventTimer);
	fsmNodeWinWalkingToChar->addEvent("win_walking_to_char", fsmNodeWinArrivedAtChar, &distToEventFunc, &m_pEventDist);
	fsmNodeWinArrivedAtChar->addEvent("win_arrived_at_char", fsmNodeWinWalkingToExit, &timedEventFunc, &m_pEventTimer);
	fsmNodeWinWalkingToExit->addEvent("win_walking_to_exit", fsmNodeWinVanish, &distToEventFunc, &m_pEventDist);
	fsmNodeWinVanish->addEvent("win_vanish", fsmNodeWinOut, &timedEventFunc, &m_pEventTimer);


	m_fsm.addNode(fsmNodeIdle);
	m_fsm.addNode(fsmNodeSeeking);
	m_fsm.addNode(fsmNodeAppealed);
	m_fsm.addNode(fsmNodeWalking);
	m_fsm.addNode(fsmNodeArrived);
	m_fsm.addNode(fsmNodeOccupiedObj);
	m_fsm.addNode(fsmNodeOccupiedCrit);
	m_fsm.addNode(fsmNodeOccupiedWorkguy);
	m_fsm.addNode(fsmNodeWinAppealed);
	m_fsm.addNode(fsmNodeWinWalkingToChar);
	m_fsm.addNode(fsmNodeWinArrivedAtChar);
	m_fsm.addNode(fsmNodeWinWalkingToExit);
	m_fsm.addNode(fsmNodeWinVanish);


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
    addState("walk_surpr1_e", 156, 163, 10, true);
    addState("walk_surpr2_e", 164, 171, 10, true);
    addState("walk_surpr1_se", 140, 147, 10, true);
    addState("walk_surpr2_se", 148, 455, 10, true);
    addState("walk_surpr1_s", 124, 131, 10, true);
    addState("walk_surpr2_s", 132, 139, 10, true);
	addState("walk_surpr1_so", 108, 115, 10, true);
	addState("walk_surpr2_so", 116, 123, 10, true);
	addState("walk_surpr1_o", 92, 99, 10, true);
	addState("walk_surpr2_o", 100, 107, 10, true);
    addState("occupied", 64, 67, 5, true);
    addState("idle_n", 68, 68, 5, true);
    addState("idle_ne", 69, 69, 1, true);
    addState("idle_e", 70, 73, 4, true);
	addState("idle_se", 74, 77, 4, true);
	addState("idle_s", 78, 82, 4, true);
	addState("idle_so", 83, 86, 4, true);
	addState("idle_o", 87, 90, 4, true);
	addState("idle_no", 91, 91, 4, true);
	addState("idle_surpr1_e", 196, 198, 10, true);
	addState("idle_surpr2_e", 199, 201, 10, true);
	addState("idle_surpr1_se", 190, 192, 10, true);
	addState("idle_surpr2_se", 193, 195, 10, true);
	addState("idle_surpr1_s", 184, 186, 10, true);
	addState("idle_surpr2_s", 187, 189, 10, true);
	addState("idle_surpr1_so", 178, 180, 10, true);
	addState("idle_surpr2_so", 181, 183, 10, true);
	addState("idle_surpr1_o", 172, 174, 10, true);
	addState("idle_surpr2_o", 175, 177, 10, true);

	updateAnimationState();

	m_pTextBubble = new TextBubble("", RscManager::get()->getFontRsc(12), 0, 0, 0, 0);
	m_pTextBubble->setActive(false);
	
	vect2df_t vStateSprPos;
	vStateSprPos.x = m_rect.getPos().x;
	vStateSprPos.y = m_rect.getPos().y - 22;

	m_pStateSprite = new AnimatedSprite(RscManager::get()->getFontRsc(13), vStateSprPos);
	m_pStateSprite->setActive(false);

	m_pStateSprite->addState("love", 0, 1, 2, true);
	m_pStateSprite->addState("talk", 2, 3, 2, true);
	m_pStateSprite->addState("attack", 4, 5, 2, true);
	m_pStateSprite->addState("money", 6, 7, 2, true);

	saySomething("Hello");
}

MainCharacter::~MainCharacter() {
	if (m_pCurrentJob != NULL) {
		delete m_pCurrentJob;
	}

	delete m_pTextBubble;
	delete m_pProgressBar;
	delete m_pStateSprite;
}

void MainCharacter::onNewState(FSM_MAINCHAR_STATE currState) {
	switch (currState) {
	case E_MAINCHAR_STATE_IDLE:
		updateAnimationState();
		m_pCurrFocusedThing = NULL;
		setEventTimer(3);

		break;

	case E_MAINCHAR_STATE_SEEKING:
		updateAnimationState();
		m_pCurrFocusedThing = NULL;

		break;

	case E_MAINCHAR_STATE_APPEALED:
		updateOrientationFromVector(vectfSub(m_rect.getPos(), m_pCurrFocusedThing->getRect()->getPos()));

		updateAnimationState();
		setEventTimer(1);

		break;

	case E_MAINCHAR_STATE_WALKING:
		updateAnimationState();
		setEventDist(8);
		
		break;

	case E_MAINCHAR_STATE_OCCUPIED_OBJ:
	case E_MAINCHAR_STATE_OCCUPIED_WORKGUY:
	{
		updateAnimationState();

		m_pEventTimer->currTime = 0;
		m_pEventTimer->limit = m_pCurrFocusedThing->getOccupationTime();

		historyThingInfo_t* pHistoryThingInfo = new historyThingInfo_t; // NE PAS OUBLIER DE LE DELETE
		pHistoryThingInfo->pThingObj = m_pCurrFocusedThing;
		pHistoryThingInfo->fCooldownTimeLeft = m_pCurrFocusedThing->getCooldownTime();

		addDataToList(&m_llThingsHistory, pHistoryThingInfo);

		onBeginUsing();
		m_pCurrFocusedThing->onBeginUsing(this);

		if (currState == E_MAINCHAR_STATE_OCCUPIED_WORKGUY) {
			m_pStateSprite->changeState(1);
			m_pStateSprite->setActive(true);
		}

		if (m_pCurrFocusedThing->isWorkThing() && hasWork()) {
			m_pStateSprite->changeState(3);
			m_pStateSprite->setActive(true);
		}

		break;
	}

	case E_MAINCHAR_STATE_OCCUPIED_CRIT:
		updateAnimationState();

		m_pNewFocusedThing = NULL;
		setEventTimer(4);

		onBeginAttack();

		m_pStateSprite->changeState(2);
		m_pStateSprite->setActive(true);

		break;

	case E_MAINCHAR_STATE_WIN_APPEALED:
		m_eCurrOrientation = getOrientationFromVector(vectfSub(m_rect.getPos(), TSGameMode::get()->getWinCharacter()->getRect()->getPos()));

		updateAnimationState();
		setEventTimer(7);

		m_pNewFocusedThing = NULL;

		break;

	case E_MAINCHAR_STATE_WIN_WALKING_TO_CHAR:
		updateAnimationState();
		setEventDist(20);

		m_pStateSprite->changeState(0);
		m_pStateSprite->setActive(true);

		break;

	case E_MAINCHAR_STATE_WIN_ARRIVED_AT_CHAR:
		updateAnimationState();
		setEventTimer(3);

		m_pStateSprite->changeState(0);
		m_pStateSprite->setActive(true);

		break;

	case E_MAINCHAR_STATE_WIN_WALKING_TO_EXIT:
		updateAnimationState();
		setEventDist(1);

		m_pStateSprite->changeState(0);
		m_pStateSprite->setActive(true);

		break;

	case E_MAINCHAR_STATE_WIN_VANISH:
		updateAnimationState();
		setEventTimer(1);

		m_pStateSprite->changeState(0);
		m_pStateSprite->setActive(true);

		break;

	case E_MAINCHAR_STATE_WIN_OUT:
		setActive(false);

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

	m_pStateSprite->setActive(false);
}

void MainCharacter::draw(uint8* buffer) {
	AnimatedSprite::draw(buffer);

	if (m_pProgressBar->isActive())
		m_pProgressBar->draw(buffer);

	m_pStateSprite->draw(buffer);

	if (m_pTextBubble->isActive())
		m_pTextBubble->draw(buffer);
}

void MainCharacter::update() {
    AnimatedSprite::update();
    
	double deltaTime = System::get()->getDeltaTime();

	m_fsm.update();

	FSM_MAINCHAR_STATE currState = getCurrentState();

	if (m_prevState != currState) {
		if (m_prevState != NULL)
			onEndState(m_prevState);

		onNewState(currState);

		m_prevState = currState;
		printf("Current state: %p: %s\n", this, m_fsm.getActiveState()->getName());
	}

	switch (currState) {
		case E_MAINCHAR_STATE_SEEKING:
		{
			m_pCurrFocusedThing = searchForAvailableThings();
			break;
		}

		case E_MAINCHAR_STATE_WALKING:
		{
			walkTo(m_pCurrFocusedThing);

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

		case E_MAINCHAR_STATE_WIN_WALKING_TO_CHAR:
			walkTo(TSGameMode::get()->getWinCharacter());

			break;

		case E_MAINCHAR_STATE_WIN_WALKING_TO_EXIT:
			walkTo(TSGameMode::get()->getWinCharacter());

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

	if (m_fBubbleTextTimeLeft > 0) {
		m_fBubbleTextTimeLeft -= deltaTime;

		if (m_fBubbleTextTimeLeft <= 0) {
			m_pTextBubble->setActive(false);
		}

		m_pTextBubble->translate(m_rect.getPos().x + (m_rect.getSize().w / 2) - (m_pTextBubble->getRect()->getSize().w / 2), m_rect.getPos().y - 15, TRANSFORM_ABS);
	}

	if (m_pStateSprite->isActive()) {
		m_pStateSprite->update();
		m_pStateSprite->getRect()->setPos(m_rect.getPos().x + 1, m_rect.getPos().y - 23);
	}
}

void MainCharacter::walkTo(vect2df_t vDstPos) {
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

void MainCharacter::walkTo(IWidget* pDstObj) {
	walkTo(pDstObj->getRect()->getPos());
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

FSM_MAINCHAR_STATE MainCharacter::getCurrentState() {
	return (FSM_MAINCHAR_STATE) m_fsm.getActiveState()->getStateId();
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

void MainCharacter::setEventTimer(int iNewTime) {
	m_pEventTimer->currTime = 0;
	m_pEventTimer->limit = iNewTime;
}

void MainCharacter::setEventDist(int iNewDistLimit) {
	m_pEventDist->currDist = 0;
	m_pEventDist->limit = iNewDistLimit;
}

void MainCharacter::showBubble(char* szStr) {
	m_pTextBubble->setText(szStr);
	m_pTextBubble->setActive(true);
	m_fBubbleTextTimeLeft = BUBBLE_STAY_TIME;
}

void MainCharacter::saySomething(char* szCategory) {
	int iNbSpeechLines = intFromStr(m_speechData.getData(szCategory, 0));

	printf("NbLines : %i\n", iNbSpeechLines);

	std::srand(std::time(NULL));
	int iRand = std::rand() % iNbSpeechLines;

	int iSpeechLineId = std::rand() % iNbSpeechLines;

	char* szSpeechLine = m_speechData.getData(szCategory, 1+iSpeechLineId);
	printf("Line id : %i : %s\n", iSpeechLineId, szSpeechLine);

	showBubble(szSpeechLine);

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
	m_pProgressBar->getRect()->setPos(m_rect.getPos().x + 2, m_rect.getPos().y - 2);
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
	TSGameMode::get()->decreaseHealth(5);
	saySomething("Attack");
}


void MainCharacter::onEndWork() {
	setHasWork(false);

	TSGameMode::get()->increaseMoney(m_pCurrentJob->m_iPrice * m_pCurrFocusedThing->getWorkEfficiency());

	delete m_pCurrentJob;
	m_pCurrentJob = NULL;
}

void MainCharacter::onThingMoved() {
	if (m_pCurrFocusedThing != NULL) {
		FSM_MAINCHAR_STATE currState = getCurrentState();

		if (currState == E_MAINCHAR_STATE_WALKING) {
			m_pCurrFocusedThing = NULL;
			m_fsm.changeState(E_MAINCHAR_STATE_SEEKING);
		}
	}
}

void MainCharacter::onWinWalk() {
	m_fsm.changeState(E_MAINCHAR_STATE_WIN_APPEALED);
}

void MainCharacter::onWinVanish() {
	m_fsm.changeState(E_MAINCHAR_STATE_WIN_VANISH);
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

	clearList(&llSortedAvailableThings);

	return pFoundThing;
}

// Static FSM events funcs
bool MainCharacter::timedEventFunc(void* arg) {
	eventTimer_t* pTimerArg = *((eventTimer_t**) arg);

	if (pTimerArg->currTime > pTimerArg->limit) {
		pTimerArg->currTime = 0;
		return true;
	}

	pTimerArg->currTime += System::get()->getDeltaTime();

	return false;
}

bool MainCharacter::distToEventFunc(void* arg) {
	eventDist_t* pEventDist = (eventDist_t*) *((eventDist_t**)arg);

	if (pEventDist->currDist < pEventDist->limit) {
		return true;
	}

	return false;
}

bool MainCharacter::validThingEventFunc(void* arg) {
	DraggableThing* pCurrThing = *((DraggableThing**) arg);

	if (pCurrThing) {
		return true;
	}

	return false;
}

bool MainCharacter::startWalkingEventFunc(void* arg) {
	return true;
}

bool MainCharacter::seenAnotherThingEventFunc(void* arg) {
	return false;
}

bool MainCharacter::determineThingIsObjFuncEvent(void* arg) {
	DraggableThing* pThing = *((DraggableThing**)arg);
	if (pThing->getClassType()->getClassTypeName() == "DraggableThing"
		|| pThing->getClassType()->getClassTypeName() == "WinningThing") {
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
	switch (getCurrentState()) {
		case E_MAINCHAR_STATE_WIN_WALKING_TO_CHAR:
		case E_MAINCHAR_STATE_WIN_WALKING_TO_EXIT:
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

		case E_MAINCHAR_STATE_OCCUPIED_OBJ:
			changeState(18);
			break;
		
		case E_MAINCHAR_STATE_WIN_ARRIVED_AT_CHAR:
		case E_MAINCHAR_STATE_OCCUPIED_WORKGUY:
		case E_MAINCHAR_STATE_OCCUPIED_CRIT:
		case E_MAINCHAR_STATE_WIN_VANISH:
		case E_MAINCHAR_STATE_IDLE:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_N:
				changeState(19);
				break;

			case E_ORIENTATION_NE:
				changeState(20);
				break;

			case E_ORIENTATION_E:
				changeState(21);
				break;

			case E_ORIENTATION_SE:
				changeState(22);
				break;

			case E_ORIENTATION_S:
				changeState(23);
				break;

			case E_ORIENTATION_SO:
				changeState(24);
				break;

			case E_ORIENTATION_O:
				changeState(25);
				break;

			case E_ORIENTATION_NO:
				changeState(26);
				break;
			}
			break;
		}

		case E_MAINCHAR_STATE_WIN_APPEALED:
		case E_MAINCHAR_STATE_APPEALED:
		{
			switch (m_eCurrOrientation) {
			case E_ORIENTATION_N:
				changeState(19);
				break;

			case E_ORIENTATION_NE:
				changeState(20);
				break;

			case E_ORIENTATION_E:
				changeState(27);
				break;

			case E_ORIENTATION_SE:
				changeState(29);
				break;

			case E_ORIENTATION_S:
				changeState(31);
				break;

			case E_ORIENTATION_SO:
				changeState(33);
				break;

			case E_ORIENTATION_O:
				changeState(35);
				break;

			case E_ORIENTATION_NO:
				changeState(26);
				break;
			}
			break;
		}
	}
}