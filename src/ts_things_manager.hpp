#ifndef THINGS_MANAGER_HPP
#define THINGS_MANAGER_HPP

#include "linked_list.hpp"
#include "ts_game_mode.hpp"


class ThingsStore;
class MainCharacter;

class DraggableThing;
class WinningThing;
class CriticalThing;
class WorkguyThing;

class ThingsManager
{
private:
	LinkedList m_llThingsList;
	LinkedList m_llThingsToDelete;
	ThingsStore* m_pStore;

public:
	ThingsManager(ThingsStore* pStore);
	~ThingsManager();

	void addThing(DraggableThing* pNewThing);
	void deleteThing(DraggableThing* pThingToDelete);

	void processThingsToDeleteList();

	void getClosestAvailableThingsToPoint(LinkedList* pllAvailableThings, vect2d_t vPos, MainCharacter* pChar = NULL);
	DraggableThing* renewThingInStore(DraggableThing* pThingToRenew);

	void replaceThingInStore(DraggableThing* pThingToReplace);

	void onCriticalThingUsed();
	void onWorkguyThingUsed();
	void onThingMoved();

	void destroyAllThings();
};

#endif