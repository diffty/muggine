#ifndef TITLEBAR_HPP
#define TITLEBAR_HPP

#define UI_TITLEBAR_HEIGHT 6

#include "../core/iwidget.hpp"

#include "widget/image_button_widget.hpp"
#include "../core/common_types.hpp"


class Titlebar : public IWidget {
public:
	Titlebar(char* szTitle, IWidget* pParentObject);
	~Titlebar();

	void addButton(SpriteSheet *pSprSht, vect2df_t vPos, int iPressedFrameId, int iReleasedFrameId, void(*pCallback)(void*) = NULL, void* pCallbackArg = NULL);

	void onPress();
	void onRelease();

	bool receiveTouchInput(vect2d_t touchPt);
	void update();
	void draw(drawbuffer* pBuffer);

	void onMove();

private:
	char* m_szTitle = NULL;
	IWidget* m_pParentObject = NULL;
	Color m_color;
	int m_state;
	vect2d_t m_vPrevCursorPosition;
	bool m_bPressedThisLoop = false;
};

#endif
