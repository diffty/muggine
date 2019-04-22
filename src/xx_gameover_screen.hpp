#ifndef XX_GAMEOVER_SCREEN
#define XX_GAMEOVER_SCREEN

#include "common_types.hpp"
#include "text.hpp"
#include "image_button_widget.hpp"


class XXGameOverScreen : public IWidget
{
public:
    XXGameOverScreen();
    
    void update();
    void draw(uint8* fb);
};


#endif
