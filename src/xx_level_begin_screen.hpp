#ifndef LD_LEVEL_BEGIN_SCREEN
#define LD_LEVEL_BEGIN_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class XXLevelBeginScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd;

public:
	XXLevelBeginScreen(int iLevelNum);
	~XXLevelBeginScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
