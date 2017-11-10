#include "ts_level_begin_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


LevelBeginScreen::LevelBeginScreen(int iLevelNum) {
	vect2df_t vTextPos;
	vTextPos.x = 135;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL", RscManager::get()->getFontRsc(4), vTextPos);

	vTextPos.x += 40;
	m_pLevelNumTextLabel = new Text(iLevelNum, RscManager::get()->getFontRsc(4), vTextPos);
}


LevelBeginScreen::~LevelBeginScreen() {
	delete m_pLevelTextLabel;
	delete m_pLevelNumTextLabel;
}

void LevelBeginScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void LevelBeginScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	m_pLevelNumTextLabel->draw(fb);
}

bool LevelBeginScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
