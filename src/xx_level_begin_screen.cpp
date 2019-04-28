#include "xx_level_begin_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


XXLevelBeginScreen::XXLevelBeginScreen(int iLevelNum) {
	m_fTimeBeforeEnd = 3;

	vect2df_t vTextPos;
	vTextPos.x = 135;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL", RscManager::get()->getFontRsc("small-font"), vTextPos);

	vTextPos.x += 40;
	m_pLevelNumTextLabel = new Text(iLevelNum, RscManager::get()->getFontRsc("small-font"), vTextPos);
}


XXLevelBeginScreen::~XXLevelBeginScreen() {
	delete m_pLevelTextLabel;
	delete m_pLevelNumTextLabel;
}

void XXLevelBeginScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void XXLevelBeginScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	m_pLevelNumTextLabel->draw(fb);
}

bool XXLevelBeginScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
