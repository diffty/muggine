#include "ts_critical_thing.hpp"
#include "ts_things_manager.hpp"


CriticalThing::CriticalThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iActionRadius, bool bUsableOnce, bool bSingleUser) :
	DraggableThing(pSprSht, uFrameNb, vPos, pThingsManager, pInputManager, iAppealPower, -1, -1, iActionRadius, bUsableOnce, bSingleUser, false, false) {

	m_classInfo.setClassTypeName("CriticalThing");
}


CriticalThing::~CriticalThing() {

}


void CriticalThing::onUsing() {
	m_pThingsManager->onCriticalThingUsed();
}
