#ifndef THINGS_MANAGER_HPP
#define THINGS_MANAGER_HPP

#include "linked_list.hpp"


class DraggableThing;
class ThingsStore;

class ThingsManager
{
private:
	LinkedList m_llThingsList;
	ThingsStore* m_pStore;

public:
	ThingsManager(ThingsStore* pStore);
	~ThingsManager();

	void addThing(DraggableThing* pNewThing);
	void getClosestAvailableThingsToPoint(LinkedList* pllAvailableThings, vect2d_t vPos);
	void renewThingInStore(DraggableThing* pThingToRenew);
};

#endif