#ifndef THINGS_STORE_ITEM_HPP
#define THINGS_STORE_ITEM_HPP

#include "iwidget.hpp"
#include "ts_draggable_thing.hpp"
#include "text.hpp"


class ThingsStoreItem : public IWidget {
private:
	Sprite* m_pBgSpr;
	DraggableThing* m_pThing;
	Text* m_pPriceLabel;

public:
	ThingsStoreItem(DraggableThing* pThing);
	~ThingsStoreItem();
	
	void draw(uint8* fb);
	void update();
	void receiveTouchInput(vect2d_t touchPt);

	void updateChildrenPosition();
	void replaceThing(DraggableThing* pNewThing);

	void updateChildren();

	DraggableThing* getThing();
};


#endif