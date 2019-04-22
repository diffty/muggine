#ifndef LD_LEVEL_FAIL_SCREEN
#define LD_LEVEL_FAIL_SCREEN


#include "common_types.hpp"
#include "text.hpp"


class XXLevelFailScreen
{
private:
	Text* m_pLevelTextLabel;
	Text* m_pLevelNumTextLabel;

	float m_fTimeBeforeEnd;

public:
	XXLevelFailScreen();
	~XXLevelFailScreen();

	void update();
	void draw(uint8* fb);

	bool doMustDisappear();
};


#endif
