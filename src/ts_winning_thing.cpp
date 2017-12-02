#include "ts_winning_thing.hpp"
#include "ts_game_mode.hpp"



WinningThing::WinningThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos) :

	DraggableThing(pSprSht, uFrameNb, vPos) {

	m_classInfo.setClassTypeName("WinningThing");

	setUsableOnce(true);
	setSingleUser(true);
}


WinningThing::~WinningThing() {
	
}

void WinningThing::onDragEnd() {
	DraggableThing::onDragEnd();

	if (!m_bTimerActivated && !m_bInStore) {
		m_bTimerActivated = true;
		m_bIsDraggable = false;
		m_bIsHoverable = false;

		TSGameMode::get()->launchWinTimer();
	}
}

void WinningThing::onEndUsing(MainCharacter* pChar) {
	DraggableThing::onEndUsing(pChar);

	TSGameMode::get()->stopWinTimer();
}
