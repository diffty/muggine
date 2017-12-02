#ifndef TS_DRAGGABLETHING_HPP
#define TS_DRAGGABLETHING_HPP

#include "sprite.hpp"
#include "image.hpp"
#include "input.hpp"
#include "draggable_sprite.hpp"
#include "tiny_progress_bar.hpp"
#include "linked_list.hpp"
#include "iobject.hpp"
#include "spritesheet.hpp";
#include "ts_text_bubble.hpp"
#include "rsc_manager.hpp"



class ThingsManager;
class MainCharacter;

class DraggableThing : public DraggableSprite
{
protected:
	int m_iAppealPower;
	int m_iOccupationTime;
	int m_iCooldownTime;
	int m_iActionRadius;
	bool m_bUsableOnce;
	bool m_bSingleUser;
	int m_iMoneyValue = -1;
	int m_iPrice = 0;
	int m_iWorkEfficiency = 0;
	bool m_bDestroyAfterUse;
	bool m_bUsableOnlyDuringWork = false;
	bool m_bIsWorkThing = false;
	bool m_bIsGrabbable = true;
	bool m_bInStore;
	bool m_bIsUsing = false;

	char* m_szTitle;
	char* m_szDesc;

	float m_fAnimOffsetCoef = 0.;

	SpriteSheet* m_pShadowSprSht;

	MainCharacter* m_pCharOwner = NULL;

	LinkedList m_llUsers;

	TextBubble* m_pTextBubble;

public:
	DraggableThing(Image* pImg, vect2df_t vPos);
	DraggableThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos);
	~DraggableThing();

	void init(char* szTitle, char* szDesc);
	void clone();

	void registerUser(MainCharacter* pCharUser);
	void unregisterUser(MainCharacter* pCharUser);

	void update();
	void draw(uint8* fb);

	bool isUsed();
	bool isInStore();
	bool isUsableOnce();
	bool isSingleUser();
	bool isUsableOnlyDuringWork();
	bool isWorkThing();

	int getAppealPower();
	int getOccupationTime();
	int getCooldownTime();
	int getActionRadius();
	int getWorkEfficiency();
	int getPrice();
	MainCharacter* getCharOwner();

	char* getTitle();
	char* getDesc();

	void setAppealPower(int iAppealPower);
	void setOccupationTime(int iOccupationTime);
	void setCooldownTime(int iCooldownTime);
	void setActionRadius(int iActionRadius);
	void setUsableOnce(bool bUsableOnce);
	void setSingleUser(bool bSingleUser);

	void setIsInStore(bool bInStore);
	void setWorkEfficiency(int iWorkEfficiency);
	void setMoneyValue(int iMoneyValue);
	void setPrice(int iPrice);
	void setCharOwner(MainCharacter* pCharOwner);
	void setDestroyAfterUse(bool bDestroyAfterUse);
	void setUsableOnlyDuringWork(bool bUsableOnlyDuringWork);
	void setIsWorkThing(bool bIsWorkThing);
	void setTitle(char* szTitle);
	void setDesc(char* szDesc);
	void updateTextBubbleContent();

	void resetAnimCoef();

	// Overrides du parent
	virtual void onDragStart(vect2d_t vStartDragPt);
	virtual void onDragEnd();
	virtual void onDragging();

	virtual void onBeginUsing(MainCharacter* pChar);
	virtual void onEndUsing(MainCharacter* pChar);
	virtual void onUsing();

	void onHoverStart(vect2d_t vStartHoverPt);
	void onHoverEnd();
	void onHovering();
};


#endif
