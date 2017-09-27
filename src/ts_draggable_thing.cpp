#include "ts_draggable_thing.hpp"
#include "ts_things_manager.hpp"


DraggableThing::DraggableThing(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser, bool bIsDraggable) :
	DraggableSprite(pImg, vPos, pInputManager, bIsDraggable) {

	init(pThingsManager, iAppealPower, iOccupationTime, bUsableOnce, bSingleUser);
}

DraggableThing::DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser, bool bIsDraggable) :
	DraggableSprite(pSprSht, uFrameNb, vPos, pInputManager, bIsDraggable) {

	init(pThingsManager, iAppealPower, iOccupationTime, bUsableOnce, bSingleUser);
}

DraggableThing::~DraggableThing() {
	
}


void DraggableThing::init(ThingsManager* pThingsManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser) {
	m_pThingsManager = pThingsManager;

	m_iAppealPower = iAppealPower;
	m_iOccupationTime = iOccupationTime;
	m_bUsableOnce = bUsableOnce;
	m_bSingleUser = bSingleUser;
}

void DraggableThing::onDragStart(vect2d_t vStartDragPt) {
	
}

void DraggableThing::onDragEnd() {
	m_pThingsManager->addThing(this);
	m_pThingsManager->renewThingInStore(this);
}

void DraggableThing::onDragging() {
	
}
