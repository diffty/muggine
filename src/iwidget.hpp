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
    bool m_bIsActive;
	bool m_bStopOnFirstInput;
	IWidget* m_pRootWidget;
	IWidget* m_pParentWidget;
	LinkedList m_llChildrenWidgets;
	LinkedList m_llWidgetNodesToDelete;
	int m_iDrawOrder;


public:
	IWidget(float x, float y, float w, float h)
		: IDrawable(),
		m_rect(x, y, w, h) {
		
        init();
	};

	IWidget(float x, float y)
		: IDrawable(),
		m_rect(x, y, 0., 0.) {
            
        init();

	};

	IWidget()
		: IDrawable(),
		m_rect(0., 0., 0., 0.) {
            
        init();
	};

	virtual ~IWidget();
    
    void init();

	Rectf* getRect() { return &m_rect; };

	virtual void update() {
		updateChildren();
	};

	virtual bool receiveTouchInput(vect2d_t touchPt) {
		return receiveTouchInputChildren(touchPt);
	};

	void drawChildren(uint8* buffer);
	void updateChildren();
	bool receiveTouchInputChildren(vect2d_t touchPt);
    
    void translate(float x, float y, ETransformMode transformMode = TRANSFORM_REL);
    
	void setActive(bool bIsActive);
	bool isActive();

	void addChildWidget(IWidget* pWidget);
	void removeChildWidget(IWidget* pWidget);
	void updateChildrenRootWidget();
	void unlinkAllWidgets();

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
