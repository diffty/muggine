#include "ld_object_card.hpp"
#include "ld_game_mode.hpp"
#include "ld_truck_content.hpp"
#include "system.hpp"


ObjectCard::ObjectCard(Image* pImg, vect2df_t vPos, bool bIsDraggable)
	: DraggableSprite(pImg, vPos, bIsDraggable) {

	setDrawOrder(5000);
	setDraggable(true);

	m_vInitPos = getRect()->getPos();

	m_pTextWidget = new Text("", RscManager::get()->getFontRsc(0), { 0, 0 });
	m_pTextWidget->setDrawOrder(getDrawOrder() + 10);
	m_pTextWidget->setParentWidget(this);

	m_pPriceWidget = new Text("", RscManager::get()->getFontRsc(0), { 0, 0 });
	m_pPriceWidget->setDrawOrder(getDrawOrder() + 10);
	m_pPriceWidget->setParentWidget(this);

	m_aLoveSprArray = new Sprite*[5];

	for (int i = 0; i < 5; i++) {
		m_aLoveSprArray[i] = new Sprite(RscManager::get()->getImgRsc(17), { 0, 0 });
		m_aLoveSprArray[i]->setDrawOrder(getDrawOrder() + 10);
		m_aLoveSprArray[i]->setParentWidget(this);
	}
}

ObjectCard::~ObjectCard() {
	delete[] m_aLoveSprArray;
}

void ObjectCard::setObject(Object* pObjectInCard) {
	if (m_pObjectInCard) {
		m_pObjectInCard->setParentWidget(NULL);
	}

	m_pObjectInCard = pObjectInCard;

	m_pObjectInCard->setIsCrate(false);

	m_pObjectInCard->setParentWidget(this);
	m_pObjectInCard->setDrawOrder(getDrawOrder() + 10);
	
	m_pTextWidget->setText(m_pObjectInCard->getTitle());
	m_pPriceWidget->setText(m_pObjectInCard->getPrice());

	// LOOOL
	char* tmpChar = new char[strlen(m_pPriceWidget->getText()) + 2];
	strcpy(tmpChar, m_pPriceWidget->getText());
	strcat(tmpChar, "$");
	m_pPriceWidget->setText(tmpChar);
	delete[] tmpChar;

	placeWidgetsInCard();
}

Object* ObjectCard::getObject() {
	return m_pObjectInCard;
}

void ObjectCard::update() {
	DraggableSprite::update();

	placeWidgetsInCard();

	/*if (System::get()->getInputSys()->IsKeyPressed(KEYB_A) && isDraggable()) {
		LDGameMode::get()->destroyObjectCard(this);
	}*/
}

void ObjectCard::placeWidgetsInCard() {
	if (m_pObjectInCard) {
		m_pObjectInCard->getRect()->setPos(getRect()->getPos().x + 7, getRect()->getPos().y + 7);

		int iLoveSprWidth = m_aLoveSprArray[0]->getRect()->getSize().w;
		int iLoveFactor = m_pObjectInCard->getLoveFactor();

		for (int i = 0; i < 5; i++) {
			m_aLoveSprArray[i]->getRect()->setPos(
				getRect()->getPos().x + 8 + (iLoveSprWidth + 1) * i,
				getRect()->getPos().y + 75
			);

			if (iLoveFactor+1 <= i) {
				m_aLoveSprArray[i]->setActive(false);
			}
		}
	}

	m_pTextWidget->getRect()->setPos(getRect()->getPos().x + 5, getRect()->getPos().y + 38);
	m_pPriceWidget->getRect()->setPos(getRect()->getPos().x + 40, getRect()->getPos().y + 10);
}

void ObjectCard::onDragEnd() {
	int iDragDist = m_vInitPos.x - getRect()->getPos().x;

	bool bDoDestroyCard = false;

	if (abs(iDragDist) > 20) {
		// setActive(false);

		if (iDragDist > 0) {
			if (!LDGameMode::get()->getTruckContent()->isFull()) {
				m_pObjectInCard->parentToRoot();
				m_pObjectInCard->goToKeep();
				bDoDestroyCard = true;
				m_pObjectInCard = NULL;
			}
		}
		else {
			m_pObjectInCard->parentToRoot();
			m_pObjectInCard->goToSell();
			m_pObjectInCard = NULL;
			bDoDestroyCard = true;
		}

		if (bDoDestroyCard) {
			m_pObjectInCard = NULL;
			LDGameMode::get()->destroyObjectCard(this);
		}
	}
	
	if (!bDoDestroyCard) {
		getRect()->setPos(m_vInitPos.x, m_vInitPos.y);
	}
}
