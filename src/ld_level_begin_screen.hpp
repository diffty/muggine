#ifndef LD_LEVEL_BEGIN_SCREEN
#define LD_LEVEL_BEGIN_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class LDLevelBeginScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd = 3;

public:
	LDLevelBeginScreen(int iLevelNum);
	~LDLevelBeginScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
