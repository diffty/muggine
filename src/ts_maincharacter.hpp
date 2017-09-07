#ifndef TS_MAINCHARACTER_HPP
#define TS_MAINCHARACTER_HPP

#include "common_types.hpp"
#include "iwidget.hpp"
#include "fsm.hpp"
#include "fsm_node.hpp"



enum FSM_MAINCHAR_STATE {
	E_MAINCHAR_STATE_IDLE,
	E_MAINCHAR_STATE_APPEALED,
	E_MAINCHAR_STATE_TALKING
};

class MainCharacter : public IWidget {

private:
	FSM m_fsm;
	int* m_pTestVar;

public:
	MainCharacter();
	~MainCharacter();

	void update();
	void draw(uint8* buffer);
};

#endif
