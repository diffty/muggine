#ifndef TS_INTERACTABLE_THING_HPP
#define TS_INTERACTABLE_THING_HPP

#include <math.h>
#include "sprite.hpp"

class InteractableThing : public Sprite {
private:
	int m_iAppealPower;
	int m_iOccupationTime;
	bool m_bUsableOnce;
	bool m_bSingleUser;

public:
	InteractableThing(Image* pImg, vect2df_t vPos, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser);
	InteractableThing(SpriteSheet* pSprSh, uint uFrameNb, vect2df_t vPos, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser);
	~InteractableThing();

	void init(int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser);
};

#endif