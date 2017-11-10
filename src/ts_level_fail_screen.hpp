#ifndef TS_LEVEL_FAIL_SCREEN
#define TS_LEVEL_FAIL_SCREEN


#include "common_types.hpp"
#include "text.hpp"


class LevelFailScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 3;

public:
	LevelFailScreen::LevelFailScreen();
	~LevelFailScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
