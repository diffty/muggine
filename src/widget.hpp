#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"


class IWidget {
protected:
	Rect m_rect;

public:
	IWidget(int x, int y, int w, int h)
		: m_rect(x, y, w, h) {

	}

	Rect* getRect() { return &m_rect; }

	virtual void receiveTouchInput(vect2d_t touchPt) {};
	virtual void update() {};
	virtual void draw(uint8* fb) = 0;
};


#endif