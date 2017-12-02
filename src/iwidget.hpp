#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"
#include "idrawable.hpp"
#include "linked_list.hpp"


enum ETransformMode {
    TRANSFORM_REL,
    TRANSFORM_ABS,
};


class IWidget : public IDrawable {
protected:
	Rectf m_rect;
    bool m_bIsActive = true;
	IWidget* m_pRootWidget = NULL;
	IWidget* m_pParentWidget = NULL;
	LinkedList m_llChildrenWidgets;
	LinkedList m_llWidgetNodesToDelete;
	int m_iDrawOrder = 0;


public:
	IWidget(float x, float y, float w, float h)
		: IDrawable(),
		m_rect(x, y, w, h) {

		initList(&m_llChildrenWidgets);
	};

	IWidget(float x, float y)
		: IDrawable(),
		m_rect(x, y, 0., 0.) {
	
		initList(&m_llChildrenWidgets);
	};

	IWidget()
		: IDrawable(),
		m_rect(0., 0., 0., 0.) {
	
		initList(&m_llChildrenWidgets);
	};

	virtual ~IWidget();

	Rectf* getRect() { return &m_rect; };

	virtual void update() {
		updateChildren();
	};

	virtual void receiveTouchInput(vect2d_t touchPt) {
		receiveTouchInputChildren(touchPt);
	};

	void drawChildren(uint8* buffer);
	void updateChildren();
	void receiveTouchInputChildren(vect2d_t touchPt);
    
    void translate(float x, float y, ETransformMode transformMode = TRANSFORM_REL);
    
	void setActive(bool bIsActive);
	bool isActive();

	void addChildWidget(IWidget* pWidget);
	void removeChildWidget(IWidget* pWidget);
	void updateChildrenRootWidget();

	void garbageCollect();

	void setRootWidget(IWidget* pRootWidget);
	void setChildrenRootWidget(IWidget* pRootWidget);
	IWidget* getRootWidget();

	void setParentWidget(IWidget* pWidget);
	IWidget* getParentWidget();

	void setDrawOrder(int iNewDrawOrder);
	int getDrawOrder();

	void addWidgetToDrawOrder(IWidget* pWidget);

	void updateWidgetInParentDrawOrder();
	void updateWidgetInDrawOrder(IWidget* pWidget);
	void updateWidgetNodeInDrawOrder(LLNode* pWidgetNodeToAdd);

	void printDrawOrder();
};

#endif
