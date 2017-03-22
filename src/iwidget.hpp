#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"
#include "idrawable.hpp"


class IWidget : public IDrawable {
protected:
	Rect m_rect;

public:
	IWidget(int x, int y, int w, int h)
		: IDrawable(),
		  m_rect(x, y, w, h) {}

	Rect* getRect() { return &m_rect; }

	virtual void update() {};
	virtual void receiveTouchInput(vect2d_t touchPt) {};
};


#endif