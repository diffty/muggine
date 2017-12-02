#include "ts_things_store_item.hpp"



ThingsStoreItem::ThingsStoreItem(DraggableThing* pThing) :
	IWidget() {

	vect2df_t vBgPos;
	vBgPos.x = 0;
	vBgPos.y = 0;
	m_pBgSpr = new Sprite(RscManager::get()->getSprShtRsc(7), 0, vBgPos);

	m_rect.setSize(m_pBgSpr->getRect()->getSize().w, m_pBgSpr->getRect()->getSize().h);

	m_pThing = pThing;

	char* szPriceStr = Text::intToStr(m_pThing->getPrice());
	m_pPriceLabel = new Text(szPriceStr, RscManager::get()->getFontRsc(5), vBgPos);
	delete szPriceStr;

	m_pBgSpr->setDrawOrder(4900);
	m_pThing->setDrawOrder(5000);
	m_pPriceLabel->setDrawOrder(5100);

	printf("f\n");
	m_pBgSpr->setParentWidget(this);
	printf("a\n");
	m_pThing->setParentWidget(this);
	printf("s\n");
	m_pPriceLabel->setParentWidget(this);

	updateChildren();
}


ThingsStoreItem::~ThingsStoreItem() {
	delete m_pThing;
	delete m_pBgSpr;
	delete m_pPriceLabel;
}

void ThingsStoreItem::draw(uint8* buffer) {
	if (m_bIsActive) {
		drawChildren(buffer);

		/*m_pBgSpr->draw(buffer);
		m_pThing->draw(buffer);
		m_pPriceLabel->draw(buffer);*/
	}
}

void ThingsStoreItem::update() {
	updateChildren();

	//m_pThing->update();
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
	IWidget::updateChildren();

	m_pBgSpr->setRootWidget(getRootWidget());
	m_pBgSpr->updateChildren();

	m_pThing->setRootWidget(getRootWidget());
	m_pThing->updateChildren();
}