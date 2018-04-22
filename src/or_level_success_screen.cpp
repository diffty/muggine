#include "or_level_success_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"
#include "or_game_mode.hpp"


ORLevelSuccessScreen::ORLevelSuccessScreen(int iLevelNum) {
	m_bDoMustDisappear = false;
}


ORLevelSuccessScreen::~ORLevelSuccessScreen() {
    
}

void ORLevelSuccessScreen::update() {
	if (m_iIsMouseBtnState == 0 && System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 1;
	}
	else if (m_iIsMouseBtnState == 1 && System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 2;
	}
	else if (m_iIsMouseBtnState > 0 && !System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 0;
	}
}


void ORLevelSuccessScreen::draw(uint8* fb) {
	
}

bool ORLevelSuccessScreen::doMustDisappear() {
	return m_bDoMustDisappear;
}
