#include "ts_interactable_thing.hpp"


InteractableThing::InteractableThing(Image* pImg, vect2df_t vPos, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser)
	: Sprite(pImg, vPos) {
	
	init(iAppealPower, iOccupationTime, bUsableOnce, bSingleUser);
}

InteractableThing::InteractableThing(SpriteSheet* pSprSh, uint uFrameNb, vect2df_t vPos, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser)
	: Sprite(pSprSh, uFrameNb, vPos) {

	init(iAppealPower, iOccupationTime, bUsableOnce, bSingleUser);

}

InteractableThing::~InteractableThing() {

}

void InteractableThing::init(int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser) {
	m_iAppealPower = iAppealPower;
	m_iOccupationTime = iOccupationTime;
	m_bUsableOnce = bUsableOnce;
	m_bSingleUser = bSingleUser;
}

