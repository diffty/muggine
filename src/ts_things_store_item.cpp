#include "ts_things_store_item.hpp"



ThingsStoreItem::ThingsStoreItem(DraggableThing* pThing) :
	IWidget() {

	vect2df_t vBgPos;
	vBgPos.x = 0;
	vBgPos.y = 0;
	m_pBgSpr = new Sprite(RscManager::get()->getSprShtRsc(7), 0, vBgPos);

	m_rect.setSize(m_pBgSpr->getRect()->getSize().w, m_pBgSpr->getRect()->getSize().h);

	m_pThing = pThing;

	m_pPriceLabel = new Text(Text::intToStr(m_pThing->getPrice()), RscManager::get()->getFontRsc(5), vBgPos);

	updateChildren();

}


ThingsStoreItem::~ThingsStoreItem() {
	delete m_pBgSpr;
	delete m_pPriceLabel;
}

void ThingsStoreItem::draw(uint8* fb) {
	m_pBgSpr->draw(fb);
	m_pThing->draw(fb);
	m_pPriceLabel->draw(fb);
}

void ThingsStoreItem::update() {
	m_pThing->update();
}

void ThingsStoreItem::receiveTouchInput(vect2d_t touchPt) {
	m_pThing->receiveTouchInput(touchPt);
}

// mmmmmmh
void ThingsStoreItem::updateChildrenPosition() {
	vect2df_t currPos = m_rect.getPos();

	m_pBgSpr->translate(currPos.x, currPos.y, TRANSFORM_ABS);
	m_pThing->translate(currPos.x + 5, currPos.y + 6, TRANSFORM_ABS);
	m_pPriceLabel->translate(currPos.x + 3, currPos.y + 16, TRANSFORM_ABS);
}

void ThingsStoreItem::replaceThing(DraggableThing* pNewThing) {
	m_pThing = pNewThing;
	updateChildrenPosition();
}

DraggableThing* ThingsStoreItem::getThing() {
	return m_pThing;
}

void ThingsStoreItem::updateChildren() {
	m_pBgSpr->setParentScene(getParentScene());
	m_pBgSpr->updateChildren();

	m_pThing->setParentScene(getParentScene());
	m_pThing->updateChildren();
}