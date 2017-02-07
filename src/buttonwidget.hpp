#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP


#include <3ds.h>
#include "common_types.hpp"
#include "drawing.hpp"
#include "widget.hpp"


enum ButtonMode {
	E_BTNMODE_INSTANT,
	E_BTNMODE_TOGGLE
};


class ButtonWidget : public IWidget {
private:
	unsigned int m_id;
	unsigned int m_state;

	Color m_colorOn;
	Color m_colorPressed;
	Color m_colorOff;

	bool m_bPressedThisLoop;

	ButtonMode m_mode;

	int (*m_pCallback)() = NULL;

public:
	ButtonWidget(int x, int y, int w, int h, int initalState, int (*pCallback)(void));

	void receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();
	void update();
	void draw(u8* fb);
};


#endif