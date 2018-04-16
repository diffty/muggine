#include "ld_object.hpp"
#include "ld_game_mode.hpp"
#include "system.hpp"


Object::Object(char* szTitle, SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, bool bIsDraggable)
	: DraggableSprite(pSprSht, uFrameNb, vPos, bIsDraggable) {

	setTitle(szTitle);
}


Object::~Object() {
	delete m_szTitle;
}

void Object::goToSell() {
	m_fAnimCoef = 0;
	m_fAnimDuration = 0.5;
	m_vAnimSrc = getRect()->getPos();
	m_vAnimDst = { 260, 40 };
	m_eAnimType = ANIM_OBJ_SELL;
	m_bAnimActive = true;
}

void Object::goToKeep() {
	m_fAnimCoef = 0;
	m_fAnimDuration = 0.5;
	m_vAnimSrc = getRect()->getPos();
	m_vAnimDst = LDGameMode::get()->getTruckContent()->getNextPosInGrid(this);
	m_eAnimType = ANIM_OBJ_KEEP;
	m_bAnimActive = true;
}

void Object::parentToRoot() {
	setParentWidget(getRootWidget());
}

void Object::setIsCrate(bool bIsCrate) {
	m_bIsCrate = bIsCrate;
}

void Object::update() {
	if (m_bAnimActive) {
		//m_fAnimCoef *= System::get()->getDeltaTime() / m_fAnimDuration;

		m_fAnimCoef += ((1. - m_fAnimCoef) / 2.) * System::get()->getDeltaTime() * 20.;

		//if (m_fAnimCoef >= 1) {
		if (m_fAnimCoef >= 0.99) {
			m_fAnimCoef = 1.;
			m_bAnimActive = false;
			onEndAnim();
		}

		translate(
			m_vAnimSrc.x + (m_vAnimDst.x - m_vAnimSrc.x) * m_fAnimCoef,
			m_vAnimSrc.y + (m_vAnimDst.y - m_vAnimSrc.y) * m_fAnimCoef,
			TRANSFORM_ABS
		);

	}
}

void Object::draw(uint8* pBuffer) {
	if (!m_bIsCrate) {
		DraggableSprite::draw(pBuffer);
	}
	else {
		vect2df_t vPos = getRect()->getPos();
		RscManager::get()->getImgRsc(15)->draw(pBuffer, vPos.x, vPos.y - getRect()->getSize().h/2, false, true);
		drawChildren(pBuffer);
	}
}

void Object::setPrice(int iPrice) {
	m_iPrice = iPrice;
}

void Object::setLoveFactor(int iLoveFactor) {
	m_iLoveFactor = iLoveFactor;
}

void Object::setTitle(char* szTitle) {
	long lStrLen = strlen(szTitle);
	m_szTitle = new char[lStrLen+1];
	memcpy(m_szTitle, szTitle, lStrLen + 1);
}

int Object::getPrice() {
	return m_iPrice;
}

int Object::getLoveFactor() {
	return m_iLoveFactor;
}

char* Object::getTitle() {
	return m_szTitle;
}

void Object::onEndAnim() {
	switch(m_eAnimType) {
	case ANIM_OBJ_KEEP:
		LDGameMode::get()->getTruckContent()->addObject(this);
		LDGameMode::get()->onObjKeep(this);
		setIsCrate(true);
		break;

	case ANIM_OBJ_SELL:
		LDGameMode::get()->onObjSold(this);
		break;
	}
}

