#ifndef WIDGET_HPP
#define WIDGET_HPP


#include "common_types.hpp"
#include "idrawable.hpp"
#include "../utils/linked_list.hpp"


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
	char* m_szName = NULL;


public:
	IWidget(float x, float y, float w, float h, char* szName=NULL)
		: IDrawable(),
		m_rect(x, y, w, h) {
		
        init(szName);
	};

	IWidget(float x, float y, char* szName = NULL)
		: IDrawable(),
		m_rect(x, y, 0., 0.) {
            
        init(szName);
	};

	IWidget(char* szName = NULL)
		: IDrawable(),
		m_rect(0., 0., 0., 0.) {
            
        init(szName);
	};

	virtual ~IWidget();
    
    void init(char* szName = NULL);

	Rectf* getRect() { return &m_rect; };
    void recomputeRect();
	
    virtual void update() {
		updateChildren();
	};

	virtual bool receiveTouchInput(vect2d_t touchPt) {
		return receiveTouchInputChildren(touchPt);
	};

	void drawChildren(drawbuffer* pBuffer);
	void updateChildren();
	bool receiveTouchInputChildren(vect2d_t touchPt);
    
    void translate(float x, float y, ETransformMode transformMode = TRANSFORM_REL);
    
	void setActive(bool bIsActive);
	bool isActive();

	void setName(char* szNewName);
	char* getName();

	void addChildWidget(IWidget* pWidget);
	void removeChildWidget(IWidget* pWidget);
    
    IWidget* getChild(int iChildId);
    IWidget* getLastChild();
    int countChildren();
    
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
