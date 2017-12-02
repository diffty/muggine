#include "ld_level_begin_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LDLevelBeginScreen::LDLevelBeginScreen(int iLevelNum) {
	vect2df_t vTextPos;
	vTextPos.x = 135;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL", RscManager::get()->getFontRsc(0), vTextPos);

	vTextPos.x += 40;
	m_pLevelNumTextLabel = new Text(iLevelNum, RscManager::get()->getFontRsc(0), vTextPos);
}


LDLevelBeginScreen::~LDLevelBeginScreen() {
	delete m_pLevelTextLabel;
	delete m_pLevelNumTextLabel;
}

void LDLevelBeginScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LDLevelBeginScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	m_pLevelNumTextLabel->draw(fb);
}

bool LDLevelBeginScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
