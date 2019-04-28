#include "xx_level_fail_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


XXLevelFailScreen::XXLevelFailScreen() {
	m_fTimeBeforeEnd = 3;

	vect2df_t vTextPos;
	vTextPos.x = 130;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL FAILED", RscManager::get()->getFontRsc("small-font"), vTextPos);
}


XXLevelFailScreen::~XXLevelFailScreen() {
	delete m_pLevelTextLabel;
}

void XXLevelFailScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void XXLevelFailScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
}

bool XXLevelFailScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
