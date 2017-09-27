#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"
#include "idrawable.hpp"


enum ETransformMode {
    TRANSFORM_REL,
    TRANSFORM_ABS,
};

class Scene;


class IWidget : public IDrawable {
protected:
	Rectf m_rect;
    bool m_bIsActive = true;
	Scene* m_pParentScene;

public:
	IWidget(float x, float y, float w, float h)
		: IDrawable(),
		m_rect(x, y, w, h) {};

	IWidget(float x, float y)
		: IDrawable(),
		m_rect(x, y, 0., 0.) {};

	IWidget()
		: IDrawable(),
		m_rect(0., 0., 0., 0.) {};

	virtual ~IWidget() {};

	Rectf* getRect() { return &m_rect; };

	virtual void update() {};
	virtual void receiveTouchInput(vect2d_t touchPt) {};

	virtual void updateChildren() {};
    
    void translate(float x, float y, ETransformMode transformMode = TRANSFORM_REL);
    
	void setActive(bool bIsActive);
	bool isActive();

	void setParentScene(Scene* pParentScene);
	Scene* getParentScene();
};

#endif
