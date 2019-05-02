#ifndef XX_GAMEOVER_SCREEN
#define XX_GAMEOVER_SCREEN

#include "../core/common_types.hpp"
#include "../ui/widget/text.hpp"
#include "../ui/widget/image_button_widget.hpp"


class XXGameOverScreen : public IWidget
{
public:
    XXGameOverScreen();
    
    void update();
    void draw(uint8* fb);
};


#endif
