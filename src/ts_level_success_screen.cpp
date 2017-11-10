#include "ts_level_success_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LevelSuccessScreen::LevelSuccessScreen(int iLevelNum) {
	vect2df_t vTextPos;
	vTextPos.x = 100;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL   CLEARED", RscManager::get()->getFontRsc(4), vTextPos);

	vTextPos.x += 40;
	m_pLevelNumTextLabel = new Text(iLevelNum, RscManager::get()->getFontRsc(4), vTextPos);
}


LevelSuccessScreen::~LevelSuccessScreen() {
	delete m_pLevelTextLabel;
	delete m_pLevelNumTextLabel;
}

void LevelSuccessScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LevelSuccessScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	m_pLevelNumTextLabel->draw(fb);
}

bool LevelSuccessScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
