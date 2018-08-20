#include "or_level_fail_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"


ORLevelFailScreen::ORLevelFailScreen() {
	m_fTimeBeforeEnd = 3;

	vect2df_t vTextPos;
	vTextPos.x = 130;
	vTextPos.y = 80;

	m_pLevelTextLabel = new Text("LEVEL FAILED", RscManager::get()->getFontRsc(0), vTextPos);
}


ORLevelFailScreen::~ORLevelFailScreen() {
	delete m_pLevelTextLabel;
}

void ORLevelFailScreen::update() {
	m_fTimeBeforeEnd -= System::get()->getDeltaTime();
}

void ORLevelFailScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
}

bool ORLevelFailScreen::doMustDisappear() {
	return m_fTimeBeforeEnd <= 0.;
}
