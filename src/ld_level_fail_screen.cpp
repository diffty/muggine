#include "ld_level_fail_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LDLevelFailScreen::LDLevelFailScreen() {
	vect2df_t vTextPos;
	vTextPos.x = 130;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("ECHEC", RscManager::get()->getFontRsc(0), vTextPos);
}


LDLevelFailScreen::~LDLevelFailScreen() {
	delete m_pLevelTextLabel;
}

void LDLevelFailScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LDLevelFailScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
}

bool LDLevelFailScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
