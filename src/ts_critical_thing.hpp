#ifndef CRITICAL_THING_HPP
#define CRITICAL_THING_HPP

#include "ts_draggable_thing.hpp"


class CriticalThing : public DraggableThing {
public:
	CriticalThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iActionRadius, bool bUsableOnce, bool bSingleUser);
	~CriticalThing();

	void onUsing();
};


#endif