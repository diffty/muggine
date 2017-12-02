#include "ts_draggable_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_main_character.hpp"
#include "ts_game_mode.hpp"


#define OBJ_HOVER_ELEVATION 5


DraggableThing::DraggableThing(Image* pImg, vect2df_t vPos) :
	DraggableSprite(pImg, vPos) {

	m_classInfo.setClassTypeName("DraggableThing");
	m_bIsHoverable = true;

}

DraggableThing::DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos) :
	DraggableSprite(pSprSht, uFrameNb, vPos) {

	m_classInfo.setClassTypeName("DraggableThing");
	m_bIsHoverable = true;

}

DraggableThing::~DraggableThing() {
	clearList(&m_llUsers);

	delete m_szTitle;
	delete m_szDesc;

	delete m_pTextBubble;
}


void DraggableThing::init(char* szTitle, char* szDesc) {
	m_pShadowSprSht = RscManager::get()->getSprShtRsc(1);

	initList(&m_llUsers);

	m_pTextBubble = new TextBubble("", RscManager::get()->getFontRsc(12), 0, 0, 0, 0);
	m_pTextBubble->setActive(false);
	m_pTextBubble->setDrawOrder(5500);
	addChildWidget(m_pTextBubble);

	m_szTitle = new char[strlen(szTitle)+1];
	memcpy(m_szTitle, szTitle, strlen(szTitle)+1);
	m_szDesc = new char[strlen(szDesc)+1];
	memcpy(m_szDesc, szDesc, strlen(szDesc)+1);

	setDrawOrder(5000);

	updateTextBubbleContent();
}

void DraggableThing::clone() {
	m_pTextBubble = new TextBubble("", RscManager::get()->getFontRsc(12), 0, 0, 0, 0);

	int iStrLen = strlen(m_szTitle);
	char* newSzTitle = new char[iStrLen + 1];
	newSzTitle[iStrLen] = '\0';
	strcpy(newSzTitle, m_szTitle);

	setTitle(newSzTitle);
	delete newSzTitle;

	iStrLen = strlen(m_szDesc);
	char* newSzDesc = new char[iStrLen + 1];
	newSzDesc[iStrLen] = '\0';
	strcpy(newSzDesc, m_szDesc);

	setDesc(newSzDesc);
	delete newSzDesc;
}

void DraggableThing::registerUser(MainCharacter* pCharUser) {
	addDataToList(&m_llUsers, pCharUser);
}

void DraggableThing::unregisterUser(MainCharacter* pCharUser) {
	LLNode* pCurrNode = m_llUsers.pHead;

	while (pCurrNode != NULL) {
		MainCharacter* pCurrCharUser = (MainCharacter*) pCurrNode->pData;

		if (pCharUser == pCurrCharUser) {
			removeNodeFromList(&m_llUsers, pCurrNode);
			delete pCurrNode;
			return;
		}

		pCurrNode = pCurrNode->pNext;
	}
}

void DraggableThing::update() {
	DraggableSprite::update();
	
	translate(m_rect.getPos().x, m_rect.getPos().y + m_fAnimOffsetCoef * OBJ_HOVER_ELEVATION, TRANSFORM_ABS);

	bool bIsHoverable = m_bIsHoverable && !m_bInStore && m_bIsDraggable;

	if (bIsHoverable && m_bIsHovered && m_fAnimOffsetCoef < 1.) {
		m_fAnimOffsetCoef += System::get()->getDeltaTime() * OBJ_HOVER_ELEVATION;
	}
	else if ((!m_bIsHovered || !bIsHoverable) && m_fAnimOffsetCoef > 0.) {
		m_fAnimOffsetCoef -= System::get()->getDeltaTime() * OBJ_HOVER_ELEVATION;
	}
	else if ((!m_bIsHovered || !bIsHoverable) && m_fAnimOffsetCoef < 0.) {
		m_fAnimOffsetCoef = 0.;
	}
	else if (m_bIsHovered && m_fAnimOffsetCoef > 1.) {
		m_fAnimOffsetCoef = 1.;
	}

	translate(m_rect.getPos().x, m_rect.getPos().y - m_fAnimOffsetCoef * OBJ_HOVER_ELEVATION, TRANSFORM_ABS);
}

void DraggableThing::draw(uint8* fb) {
	if (m_pShadowSprSht && (!m_bInStore || m_bIsGrabbed)) {
		m_pShadowSprSht->draw(fb, 7 + (int) (m_fAnimOffsetCoef * (OBJ_HOVER_ELEVATION - 2)), m_rect.getPos().x, m_rect.getPos().y + m_fAnimOffsetCoef * OBJ_HOVER_ELEVATION, false, true);
	}

	DraggableSprite::draw(fb);

	//if (m_bInStore)
		//m_pTextBubble->draw(fb);
}

bool DraggableThing::isUsed() {
	return m_llUsers.size > 0;
}

bool DraggableThing::isInStore() {
	return m_bInStore;
}

bool DraggableThing::isUsableOnce() {
	return m_bUsableOnce;
}

bool DraggableThing::isSingleUser() {
	return m_bSingleUser;
}

bool DraggableThing::isUsableOnlyDuringWork() {
	return m_bUsableOnlyDuringWork;
}

bool DraggableThing::isWorkThing() {
	return m_bIsWorkThing;
}

int DraggableThing::getAppealPower() {
	return m_iAppealPower;
}

int DraggableThing::getOccupationTime() {
	return m_iOccupationTime;
}

int DraggableThing::getCooldownTime() {
	return m_iCooldownTime;
}

int DraggableThing::getActionRadius() {
	return m_iActionRadius;
}

int DraggableThing::getWorkEfficiency() {
	return m_iWorkEfficiency;
}

int DraggableThing::getPrice() {
	return m_iPrice;
}

MainCharacter* DraggableThing::getCharOwner() {
	return m_pCharOwner;
}

char* DraggableThing::getTitle() {
	return m_szTitle;
}

char* DraggableThing::getDesc() {
	return m_szDesc;
}

void DraggableThing::setAppealPower(int iAppealPower) {
	m_iAppealPower = iAppealPower;
}

void DraggableThing::setOccupationTime(int iOccupationTime) {
	m_iOccupationTime = iOccupationTime;
}

void DraggableThing::setCooldownTime(int iCooldownTime) {
	m_iCooldownTime = iCooldownTime;
}

void DraggableThing::setActionRadius(int iActionRadius) {
	m_iActionRadius = iActionRadius;
}

void DraggableThing::setUsableOnce(bool bUsableOnce) {
	m_bUsableOnce = bUsableOnce;
}

void DraggableThing::setSingleUser(bool bSingleUser) {
	m_bSingleUser = bSingleUser;
}

void DraggableThing::setIsInStore(bool bInStore) {
	m_bInStore = bInStore;
}

void DraggableThing::setWorkEfficiency(int iWorkEfficiency) {
	m_iWorkEfficiency = iWorkEfficiency;
}

void DraggableThing::setMoneyValue(int iMoneyValue) {
	m_iMoneyValue = iMoneyValue;
}

void DraggableThing::setPrice(int iPrice) {
	m_iPrice = iPrice;
}

void DraggableThing::setCharOwner(MainCharacter* pCharOwner) {
	m_pCharOwner = pCharOwner;
}

void DraggableThing::setDestroyAfterUse(bool bDestroyAfterUse) {
	m_bDestroyAfterUse = bDestroyAfterUse;
}

void DraggableThing::setUsableOnlyDuringWork(bool bUsableOnlyDuringWork) {
	m_bUsableOnlyDuringWork = bUsableOnlyDuringWork;
}

void DraggableThing::setIsWorkThing(bool bIsWorkThing) {
	m_bIsWorkThing = bIsWorkThing;
}


// ATTENTION GROS LEAK
void DraggableThing::setTitle(char* szTitle) {
	//delete m_szTitle;

	int iStrLen = strlen(szTitle);
	m_szTitle = new char[iStrLen + 1];
	m_szTitle[iStrLen] = '\0';

	strcpy(m_szTitle, szTitle);

	updateTextBubbleContent();
}

// ATTENTION GROS LEAK
void DraggableThing::setDesc(char* szDesc) {
	//delete m_szDesc;

	int iStrLen = strlen(szDesc);
	m_szDesc = new char[iStrLen + 1];
	m_szDesc[iStrLen] = '\0';

	strcpy(m_szDesc, szDesc);

	updateTextBubbleContent();
}

void DraggableThing::updateTextBubbleContent() {
	char* szConcatText = new char[strlen(m_szTitle) + strlen(m_szDesc) + 3];
	szConcatText[0] = '\0';

	strcat(szConcatText, m_szTitle);
	strcat(szConcatText, "\n\n");
	strcat(szConcatText, m_szDesc);

	m_pTextBubble->setText(szConcatText);

	delete szConcatText;
}

void DraggableThing::resetAnimCoef() {
	m_fAnimOffsetCoef = 0.;
}

void DraggableThing::onDragStart(vect2d_t vStartDragPt) {
	//m_bIsHoverable = true;  // alors si je mets des tooltip ça peut être emmerdant donc faudra faire un autre attr
	m_pTextBubble->setActive(false);
	setDrawOrder(5450);
}

void DraggableThing::onDragEnd() {
	ThingsManager* pThingsManager = TSGameMode::get()->getThingsManager();

	pThingsManager->onThingMoved();

	if (m_bInStore) {
		if (TSGameMode::get()->getMoney() - m_iPrice >= 0 && 
			(m_rect.getPos().x < 256 && m_rect.getPos().y < 191 &&
			 m_rect.getPos().x > 32 && m_rect.getPos().y > 32)) {

			pThingsManager->addThing(this);

			DraggableThing* pClonedThing = pThingsManager->renewThingInStore(this);

			setParentWidget(NULL);

			//pClonedThing->m_bIsHoverable = false;
			pClonedThing->resetAnimCoef();

			m_bInStore = false;
			
			TSGameMode::get()->decreaseMoney(m_iPrice);
		}
		else {
			pThingsManager->replaceThingInStore(this);
			//m_bIsHoverable = false;
			resetAnimCoef();
		}
	}

	m_pTextBubble->setActive(false);

	printf("dropped\n");
	setDrawOrder(5000 + getRect()->getPos().y);
}

void DraggableThing::onDragging() {
	
}

void DraggableThing::onBeginUsing(MainCharacter* pChar) {
	//m_bIsHoverable = false;
	m_bIsDraggable = false;

	//registerUser(pChar);
}

void DraggableThing::onEndUsing(MainCharacter* pChar) {
	if (m_iMoneyValue != -1)
		TSGameMode::get()->increaseMoney(m_iMoneyValue);

	// printf("%i, %i\n", m_bIsWorkThing, pChar->hasWork());

	unregisterUser(pChar);

	if (m_bIsWorkThing && pChar->hasWork()) {
		// printf("c ok\n");
		pChar->onEndWork();
	}

	if (m_bDestroyAfterUse) {
		m_bIsActive = false;
		TSGameMode::get()->getThingsManager()->deleteThing(this);
		// TODO: faire un appel au thingsmanager pour qu'il delete l'objet au prochain update
	}
	
	if (!isUsed()) {
		//m_bIsHoverable = true;
		m_bIsDraggable = true;
	}
}

void DraggableThing::onUsing() {
	
}

void DraggableThing::onHoverStart(vect2d_t vStartHoverPt) {
	if (m_bInStore) {
		m_pTextBubble->setActive(true);
		m_pTextBubble->translate(getRect()->getPos().x - m_pTextBubble->getRect()->getSize().w - 10, getRect()->getPos().y, TRANSFORM_ABS);
	}
}

void DraggableThing::onHoverEnd() {
	if (m_bInStore) {
		m_pTextBubble->setActive(false);
	}
}

void DraggableThing::onHovering() {
	
}