#ifndef LD_TRUCK_CONTENT_HPP
#define LD_TRUCK_CONTENT_HPP

#include "grid_widget_layout.hpp"
#include "sprite.hpp"
#include "ld_object.hpp"
#include "linked_list.hpp"


class TruckContent : public GridWidgetLayout
{
private:
	Image* m_pSlotBgImg = NULL;

public:
	TruckContent(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~TruckContent();

	void addObject(Object* pObj);

	LinkedList getObjectsList();

	void draw(uint8* pBuffer); 

	bool isFull();
};

#endif
