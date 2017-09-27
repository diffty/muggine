#ifndef THINGS_STORE_HPP
#define THINGS_STORE_HPP

#include "paginated_widget_layout.hpp"
#include "ts_draggable_thing.hpp"
#include "scene.hpp"


class ThingsStore : public PaginatedWidgetLayout
{
public:
	ThingsStore(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~ThingsStore();
	
	void renewThing(DraggableThing* pThingToRenew);
};


#endif