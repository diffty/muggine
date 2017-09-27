#ifndef TS_DRAGGABLETHING_HPP
#define TS_DRAGGABLETHING_HPP

#include "sprite.hpp"
#include "image.hpp"
#include "input.hpp"
#include "draggable_sprite.hpp"


class ThingsManager;

class DraggableThing : public DraggableSprite
{
private:
	ThingsManager* m_pThingsManager;

	int m_iAppealPower;
	int m_iOccupationTime;
	bool m_bUsableOnce;
	bool m_bSingleUser;

public:
	DraggableThing(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser, bool bIsDraggable = true);
	DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser, bool bIsDraggable = true);
	~DraggableThing();

	void init(ThingsManager* pThingsManager, int iAppealPower, int iOccupationTime, bool bUsableOnce, bool bSingleUser);

	// Overrides du parent
	void onDragStart(vect2d_t vStartDragPt);
	void onDragEnd();
	void onDragging();
};


#endif
