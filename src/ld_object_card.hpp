#ifndef LD_OBJECT_CARD_HPP
#define LD_OBJECT_CARD_HPP

#include "draggable_sprite.hpp"
#include "ld_object.hpp"
#include "image.hpp"
#include "text.hpp"


class ObjectCard : public DraggableSprite
{
private:
	Object* m_pObjectInCard = NULL;
	vect2df_t m_vInitPos;

	Text* m_pTextWidget;
	Text* m_pPriceWidget;

	Sprite** m_aLoveSprArray;

public:
	ObjectCard(Image* pImg, vect2df_t vPos, bool bIsDraggable);
	~ObjectCard();

	void setObject(Object* pObjectInCard);
	Object* getObject();

	void update();
	void placeWidgetsInCard();

	void onDragEnd();
};

#endif
