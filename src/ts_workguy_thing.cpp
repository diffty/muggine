#include "ts_workguy_thing.hpp"
#include "ts_things_manager.hpp"


WorkguyThing::WorkguyThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser) :
	DraggableThing(pSprSht, uFrameNb, vPos, pThingsManager, pInputManager, iAppealPower, iOccupationTime, iCooldownTime, iActionRadius, bUsableOnce, bSingleUser, false, false) {
		
		m_classInfo.setClassTypeName("WorkguyThing");
}


WorkguyThing::~WorkguyThing() {

}


void WorkguyThing::onUsing() {
	m_pThingsManager->onWorkguyThingUsed();
}

void WorkguyThing::onEndUsing(MainCharacter* pChar) {
	DraggableThing::onEndUsing(pChar);

	pChar->assignNewJob(this, 50);

	// Faire apparaitre le travail (un draggable thing éphémère qui fait gagner de l'argent une fois complété)

	/*vect2df_t workThingPos;
	workThingPos.x = m_rect.getPos().x;
	workThingPos.y = m_rect.getPos().y + 20;

	DraggableThing* workThing = new DraggableThing(RscManager::get()->getSprShtRsc(1), 7, workThingPos, m_pThingsManager, System::get()->getInputSys(), 1, 10, -1, 50, false, true, false, false);

	// Assigner le travail au MainCharacter correspondant
	workThing->setCharOwner(pChar);
	workThing->setMoneyValue(50);
	workThing->setDestroyAfterUse(true);

	m_pThingsManager->addThing(workThing);
	getParentScene()->addComponent(workThing);*/
}
