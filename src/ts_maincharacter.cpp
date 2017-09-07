#include "ts_maincharacter.hpp"

bool test(void* pCbArg) {
	int* testVar = (int*) *((int**)pCbArg);

	if (*testVar > 20000) {
		*testVar = 0;
		return true;
	}
	return false;
}

bool test2(void* pCbArg) {
	int* testVar = (int*) *((int**)pCbArg);

	if (*testVar > 25000) {
		return true;
	}
	return false;
}



MainCharacter::MainCharacter() {
	// Creating FSM
	m_pTestVar = new int(0);

	FSMNode* fsmNodeIdle = new FSMNode(E_MAINCHAR_STATE_IDLE, "idle");
	FSMNode* fsmNodeAppealed = new FSMNode(E_MAINCHAR_STATE_APPEALED, "appealed");
	FSMNode* fsmNodeTalking = new FSMNode(E_MAINCHAR_STATE_TALKING, "talking");

	fsmNodeIdle->addEvent("appeal", fsmNodeAppealed, &test, &m_pTestVar);
	fsmNodeAppealed->addEvent("bored", fsmNodeIdle, &test, &m_pTestVar);

	m_fsm.addNode(fsmNodeIdle);
	m_fsm.addNode(fsmNodeAppealed);
	m_fsm.addNode(fsmNodeTalking);
}

MainCharacter::~MainCharacter() {

}

void MainCharacter::update() {
	m_fsm.update();
	(*m_pTestVar)++;

	FSMNode* node = m_fsm.getActiveState();

	printf("Current state: %s\n", node->getName());
}

void MainCharacter::draw(uint8* buffer) {

}