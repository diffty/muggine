#ifndef FLOATING_WINDOW_HPP
#define FLOATING_WINDOW_HPP

#include "../core/iwidget.hpp"

class Titlebar;


class FloatingWindow :
	public IWidget
{
public:
	FloatingWindow(int x, int y, int w, int h, IWidget* pContentWidget = NULL);
	~FloatingWindow();

	void setContent(IWidget* pWidget);

	void update();
	void draw(drawbuffer* pBuffer);

	void close();
	void onMove(vect2d_t vDeltaPos);

	static void onCloseBtnPressed(void* pWindow);


private:
	IWidget* m_pContentWidget = NULL;
	Titlebar* m_pTitlebar;
	Color m_bgColor;
};

#endif
