#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"
#include "image_button_widget.hpp"


class ORLevelSuccessScreen
{
private:
	bool m_bDoMustDisappear = false;

	int m_iIsMouseBtnState = 0;

public:
	ORLevelSuccessScreen(int iLevelNum);
	~ORLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

    bool doMustDisappear();
};


#endif
