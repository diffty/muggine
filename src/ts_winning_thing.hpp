#ifndef TS_WINNING_THING
#define TS_WINNING_THING

#include "ts_draggable_thing.hpp"


class WinningThing : public DraggableThing
{
private:
	bool m_bTimerActivated = false;

public:
	WinningThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos);
	~WinningThing();

	void onDragEnd();
	void onEndUsing(MainCharacter* pChar);
};

#endif