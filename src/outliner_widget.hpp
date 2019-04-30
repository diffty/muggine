#ifndef OUTLINER_WIDGET_HPP
#define OUTLINER_WIDGET_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include "common_types.hpp"
#include "drawing.hpp"
#include "iwidget.hpp"
#include "scene.hpp"
#include "mathf.hpp"
#include "text.hpp"


class WidgetRow : public IWidget {
public:
	WidgetRow(int x, int y, int w, int h, IWidget* pWidget, WidgetRow* pParentRowWidget);
	~WidgetRow();

	bool receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();

	void update();
	void draw(uint8* pBuffer);

private:
	IWidget* m_pWidget = NULL;
	WidgetRow* m_pParentRowWidget = NULL;

	bool m_bPressedThisLoop = false;
	bool m_bIsPressed = false;

	Text* m_pTextWidget;
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
};


#endif
