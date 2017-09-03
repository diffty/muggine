#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"
#include "idrawable.hpp"


enum ETransformMode {
    TRANSFORM_REL,
    TRANSFORM_ABS,
};


class IWidget : public IDrawable {
protected:
	Rect m_rect;
    bool m_bIsActive = true;

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
    
    void translate(int x, int y, ETransformMode transformMode = TRANSFORM_REL) {
        vect2d_t currPos = m_rect.getPos();
        if (transformMode == TRANSFORM_REL) {
            currPos.x += x;
            currPos.y += y;
        }
        else {
            currPos.x = x;
            currPos.y = y;
        }
        m_rect.setPos(currPos.x, currPos.y);
    };
    
    void setActive(bool bIsActive) {
        m_bIsActive = bIsActive;
    };
    
    bool isActive() {
        return m_bIsActive;
    };

};

#endif
