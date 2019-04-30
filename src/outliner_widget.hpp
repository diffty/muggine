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


class OutlinerWidget : public IWidget {
private:
	Scene* m_pRootScene = NULL;
	bool m_bPressedThisLoop = false;
	bool m_bIsPressed = false;
	

public:
	OutlinerWidget(int x, int y, int w, int h, Scene* pRootScene);

	bool receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();

	void update();
	void draw(uint8* buffer);
};


#endif
