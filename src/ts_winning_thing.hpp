#ifndef TS_WINNING_THING
#define TS_WINNING_THING

#include "ts_draggable_thing.hpp"
#include "ts_game_mode.hpp"


class WinningThing : public DraggableThing
{
private:
	bool m_bTimerActivated = false;

public:
	WinningThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable, bool bInStore);
	~WinningThing();

	void onDragEnd();
	void onEndUsing(MainCharacter* pChar);
};

#endif