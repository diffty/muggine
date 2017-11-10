#ifndef TS_LEVEL_BEGIN_SCREEN
#define TS_LEVEL_BEGIN_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class LevelBeginScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 3;

public:
	LevelBeginScreen::LevelBeginScreen(int iLevelNum);
	~LevelBeginScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
