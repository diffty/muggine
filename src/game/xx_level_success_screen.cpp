#include "xx_level_success_screen.hpp"

#include "../manager/rsc_manager.hpp"
#include "../core/system.hpp"
#include "../game/game_mode.hpp"


XXLevelSuccessScreen::XXLevelSuccessScreen(int iLevelNum) {
	m_bDoMustDisappear = false;
	m_iIsMouseBtnState = 0;
}


XXLevelSuccessScreen::~XXLevelSuccessScreen() {
    
}

void XXLevelSuccessScreen::update() {
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


void XXLevelSuccessScreen::draw(drawbuffer* pBuffer) {
	
}

bool XXLevelSuccessScreen::doMustDisappear() {
	return m_bDoMustDisappear;
}
