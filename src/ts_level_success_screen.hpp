#ifndef TS_LEVEL_SUCCESS_SCREEN
#define TS_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class LevelSuccessScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 5;

public:
	LevelSuccessScreen::LevelSuccessScreen(int iLevelNum);
	~LevelSuccessScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif