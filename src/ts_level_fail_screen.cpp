#include "ts_level_fail_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LevelFailScreen::LevelFailScreen() {
	vect2df_t vTextPos;
	vTextPos.x = 130;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("ECHEC", RscManager::get()->getFontRsc(4), vTextPos);
}


LevelFailScreen::~LevelFailScreen() {
	delete m_pLevelTextLabel;
}

void LevelFailScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LevelFailScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
}

bool LevelFailScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
