#include "ld_level_success_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LDLevelSuccessScreen::LDLevelSuccessScreen(int iLevelNum) {
	vect2df_t vTextPos;
	vTextPos.x = 100;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL   CLEARED", RscManager::get()->getFontRsc(4), vTextPos);

	vTextPos.x += 40;
	m_pLevelNumTextLabel = new Text(iLevelNum, RscManager::get()->getFontRsc(4), vTextPos);
}


LDLevelSuccessScreen::~LDLevelSuccessScreen() {
	delete m_pLevelTextLabel;
	delete m_pLevelNumTextLabel;
}

void LDLevelSuccessScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LDLevelSuccessScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	m_pLevelNumTextLabel->draw(fb);
}

bool LDLevelSuccessScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
