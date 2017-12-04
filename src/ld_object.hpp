#ifndef LD_DRAGGABLE_SPRITE_HPP
#define LD_DRAGGABLE_SPRITE_HPP

#include "draggable_sprite.hpp"

enum EANIM_TYPE {
	ANIM_OBJ_SELL,
	ANIM_OBJ_KEEP
};

class Object : public DraggableSprite {
private:
	int m_iPrice = 0;
	int m_iLoveFactor = 0;
	char* m_szTitle;

	float m_bAnimActive = false;
	float m_fAnimCoef = 0;
	float m_fAnimDuration = 1;
	vect2df_t m_vAnimSrc;
	vect2df_t m_vAnimDst;
	EANIM_TYPE m_eAnimType;

public:
	Object(char* szTitle, SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, bool bIsDraggable = true);
	~Object();

	void goToSell();
	void goToKeep();
	void parentToRoot();

	void update();

	void setPrice(int iPrice);
	void setLoveFactor(int iLoveFactor);
	void setTitle(char* szTitle);

	int getPrice();
	int getLoveFactor();
	char* getTitle();

	void onEndAnim();
};

#endif
