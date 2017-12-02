#include "ts_critical_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_game_mode.hpp"


CriticalThing::CriticalThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iActionRadius, bool bUsableOnce, bool bSingleUser) :
	DraggableThing(pSprSht, uFrameNb, vPos) {

	m_classInfo.setClassTypeName("CriticalThing");

	setUsableOnce(false);
	setSingleUser(false);
}


CriticalThing::~CriticalThing() {

}


void CriticalThing::onUsing() {
	TSGameMode::get()->getThingsManager()->onCriticalThingUsed();
}
