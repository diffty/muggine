#ifndef OUTLINER_WIDGET_HPP
#define OUTLINER_WIDGET_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include "../../core/common_types.hpp"
#include "../../graphics/drawing.hpp"
#include "../../core/iwidget.hpp"
#include "../../graphics/scene.hpp"
#include "../../utils/mathf.hpp"
#include "../../utils/linked_list.hpp"
#include "text.hpp"


class RowWidget : public IWidget {
public:
	RowWidget(int x, int y, int w, int h, IWidget* pWidget, RowWidget* pParentRowWidget);
	~RowWidget();

	bool receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();
	void onHover();

	bool isHovered();

	void update();
	void draw(uint8* pBuffer);

private:
	IWidget* m_pWidget = NULL;
	RowWidget* m_pParentRowWidget = NULL;

	bool m_bPressedThisLoop = false;
	bool m_bIsPressed = false;
	bool m_bIsHovered = false;

	Text* m_pTextWidget;

	Color m_hoverColor;
};



class OutlinerWidget : public IWidget {
public:
	OutlinerWidget(int x, int y, int w, int h, Scene* pRootScene);
	~OutlinerWidget();

	void populate();

	bool receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();

	void update();
	void draw(uint8* buffer);

	void addRow(IWidget* pWidget);

private:
	Scene* m_pRootScene = NULL;
	bool m_bPressedThisLoop = false;
	bool m_bIsPressed = false;
	LinkedList m_llListRows;
};


#endif
