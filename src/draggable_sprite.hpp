#ifndef TS_DRAGGABLEWIDGET_HPP
#define TS_DRAGGABLEWIDGET_HPP

#include "sprite.hpp"
#include "image.hpp"
#include "input.hpp"


class DraggableSprite : public Sprite
{
public:
	DraggableSprite(Image* pImg, vect2df_t vPos, Input* pInputManager, bool bIsDraggable = true);
	DraggableSprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, Input* pInputManager, bool bIsDraggable = true);
	~DraggableSprite();

	void init(Input* pInputManager, bool bIsDraggable = true);
	void update();
	void receiveTouchInput(vect2d_t touchPt);

	bool isDraggable();

	void setDraggable(bool bIsDraggable);

	virtual void onDragStart(vect2d_t vStartDragPt) {};
	virtual void onDragEnd() {};
	virtual void onDragging() {};

protected:
	bool m_bIsGrabbed;

	Input* m_pInputManager;

	vect2d_t m_vCurrMousePos;
	vect2d_t m_vGrabPos;

	bool m_bIsDraggable;
};


#endif