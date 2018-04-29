#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"
#include "image_button_widget.hpp"


class ORLevelSuccessScreen
{
private:
	bool m_bDoMustDisappear;
	int m_iIsMouseBtnState;

public:
	ORLevelSuccessScreen(int iLevelNum);
	~ORLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

    bool doMustDisappear();
};


#endif
