#ifndef OR_GAMEOVER_SCREEN
#define OR_GAMEOVER_SCREEN

#include "common_types.hpp"
#include "text.hpp"
#include "image_button_widget.hpp"


class ORGameOverScreen : public IWidget
{
private:
    Sprite* m_pEndImgSpr;
    
public:
	ORGameOverScreen();

	void update();
	void draw(uint8* fb);
};


#endif
