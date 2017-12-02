#include "ts_workguy_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_main_character.hpp"


WorkguyThing::WorkguyThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos) :
	DraggableThing(pSprSht, uFrameNb, vPos) {
		
	setDraggable(false);
	setIsInStore(false);
	
	m_classInfo.setClassTypeName("WorkguyThing");

	m_bIsHoverable = false;
}


WorkguyThing::~WorkguyThing() {

}


void WorkguyThing::onUsing() {
	TSGameMode::get()->getThingsManager()->onWorkguyThingUsed();
}

void WorkguyThing::onEndUsing(MainCharacter* pChar) {
	DraggableThing::onEndUsing(pChar);

	pChar->assignNewJob(this, 50);

	m_bIsHoverable = false;
	m_bIsDraggable = false;

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
	getRootWidget()->addComponent(workThing);*/
}

