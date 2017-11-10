#include "ts_winning_thing.hpp"



WinningThing::WinningThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable, bool bInStore) :

	DraggableThing(pSprSht, uFrameNb, vPos, pThingsManager, pInputManager, iAppealPower, iOccupationTime, iCooldownTime, iActionRadius, bUsableOnce, bSingleUser, true, true) {

	m_classInfo.setClassTypeName("WinningThing");
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
