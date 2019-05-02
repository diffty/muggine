#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "../core/common_types.hpp"
#include "../ui/widget/text.hpp"
#include "../ui/widget/image_button_widget.hpp"


class XXLevelSuccessScreen
{
private:
	bool m_bDoMustDisappear;
	int m_iIsMouseBtnState;

public:
	XXLevelSuccessScreen(int iLevelNum);
	~XXLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

    bool doMustDisappear();
};


#endif
