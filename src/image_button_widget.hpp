#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include "common_types.hpp"
#include "drawing.hpp"
#include "sprite.hpp"


enum ButtonMode {
	E_BTNMODE_INSTANT,
	E_BTNMODE_TOGGLE
};


class ImageButtonWidget : public Sprite {
private:
	unsigned int m_state;

	bool m_bPressedThisLoop;

	int m_iPressedFrameId;
	int m_iReleasedFrameId;

	ButtonMode m_mode;

	void (*m_pCallback)(void*) = NULL;
	void *m_pCallbackArg = NULL;

public:
	ImageButtonWidget(SpriteSheet *pSprSht, vect2df_t vPos, int iPressedFrameId, int iReleasedFrameId, int initalState, void(*pCallback)(void*) = NULL, void* pCallbackArg = NULL);

	void receiveTouchInput(vect2d_t touchPt);
	void onPress();
	void onRelease();

	void update();
};


#endif