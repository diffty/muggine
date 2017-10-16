#ifndef THINGS_MANAGER_HPP
#define THINGS_MANAGER_HPP

#include "linked_list.hpp"
#include "ts_game_mode.hpp"


class DraggableThing;
class ThingsStore;
class MainCharacter;


class ThingsManager
{
private:
	LinkedList m_llThingsList;
	ThingsStore* m_pStore;

public:
	ThingsManager(ThingsStore* pStore);
	~ThingsManager();

	void addThing(DraggableThing* pNewThing);
	void getClosestAvailableThingsToPoint(LinkedList* pllAvailableThings, vect2d_t vPos, MainCharacter* pChar = NULL);
	void renewThingInStore(DraggableThing* pThingToRenew);
	void replaceThingInStore(DraggableThing* pThingToReplace);

	void onCriticalThingUsed();
	void onWorkguyThingUsed();
	void onThingMoved();
};

#endif