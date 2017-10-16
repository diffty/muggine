#ifndef THINGS_STORE_PAGE_HPP
#define THINGS_STORE_PAGE_HPP

#include "grid_widget_layout.hpp"
#include "ts_draggable_thing.hpp"
#include "linked_list.hpp"
#include "scene.hpp"
#include "ts_things_store_item.hpp"


class ThingsStorePage :	public GridWidgetLayout {
public:
	ThingsStorePage(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~ThingsStorePage();

	void renewThing(DraggableThing* pThingToRenew);
	uint countWidgets();
	int getThingWidgetIdInLayout(DraggableThing* pThing);
	ThingsStoreItem* getThingItemInLayout(DraggableThing* pThing);
	void updateChildren();
};


#endif