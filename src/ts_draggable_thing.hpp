#ifndef TS_DRAGGABLETHING_HPP
#define TS_DRAGGABLETHING_HPP

#include "sprite.hpp"
#include "image.hpp"
#include "input.hpp"
#include "draggable_sprite.hpp"
#include "tiny_progress_bar.hpp"
#include "linked_list.hpp"
#include "iobject.hpp"



class ThingsManager;
class MainCharacter;

class DraggableThing : public DraggableSprite
{
protected:
	ThingsManager* m_pThingsManager;

	int m_iAppealPower;
	int m_iOccupationTime;
	int m_iCooldownTime;
	int m_iActionRadius;
	bool m_bUsableOnce;
	bool m_bSingleUser;
	int m_iMoneyValue = -1;
	int m_iPrice = 0;
	bool m_bDestroyAfterUse;
	bool m_bUsableOnlyDuringWork = false;
	bool m_bIsWorkThing = false;

	MainCharacter* m_pCharOwner = NULL;

	bool m_bInStore;

	LinkedList m_llUsers;

public:
	DraggableThing(Image* pImg, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable = true, bool bInStore = false);
	DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, ThingsManager* pThingsManager, Input* pInputManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bIsDraggable = true, bool bInStore = false);
	~DraggableThing();

	void init(ThingsManager* pThingsManager, int iAppealPower, int iOccupationTime, int iCooldownTime, int iActionRadius, bool bUsableOnce, bool bSingleUser, bool bInStore);

	void registerNewUser(MainCharacter* pCharUser);
	void unregisterNewUser(MainCharacter* pCharUser);
	bool isUsed();

	void setIsInStore(bool bInStore);
	bool isInStore();
	bool isSingleUser();
	bool isUsableOnlyDuringWork();
	bool isWorkThing();

	int getPrice();
	MainCharacter* getCharOwner();

	void setMoneyValue(int iMoneyValue);
	void setPrice(int iPrice);
	void setCharOwner(MainCharacter* pCharOwner);
	void setDestroyAfterUse(bool bDestroyAfterUse);
	void setUsableOnlyDuringWork(bool bUsableOnlyDuringWork);
	void setIsWorkThing(bool bIsWorkThing);

	// Overrides du parent
	virtual void onDragStart(vect2d_t vStartDragPt);
	virtual void onDragEnd();
	virtual void onDragging();

	virtual void onBeginUsing();
	virtual void onEndUsing(MainCharacter* pChar);
	virtual void onUsing();

	int getOccupationTime();
	int getActionRadius();
	int getCooldownTime();
};


#endif
