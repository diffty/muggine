#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class LDLevelSuccessScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 5;

public:
	LDLevelSuccessScreen(int iLevelNum);
	~LDLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
