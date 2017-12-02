#ifndef LD_LEVEL_FAIL_SCREEN
#define LD_LEVEL_FAIL_SCREEN


#include "common_types.hpp"
#include "text.hpp"


class LDLevelFailScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 3;

public:
	LDLevelFailScreen::LDLevelFailScreen();
	~LDLevelFailScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
