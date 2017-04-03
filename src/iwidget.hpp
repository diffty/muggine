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
		m_rect(x, y, w, h) {};

	IWidget(int x, int y)
		: IDrawable(),
		m_rect(x, y, 0, 0) {};

	IWidget()
		: IDrawable(),
		m_rect(0, 0, 0, 0) {};

	virtual ~IWidget() {};

	Rect* getRect() { return &m_rect; };

	virtual void update() {};
	virtual void receiveTouchInput(vect2d_t touchPt) {};
};


#endif