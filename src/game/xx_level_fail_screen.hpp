#ifndef LD_LEVEL_FAIL_SCREEN
#define LD_LEVEL_FAIL_SCREEN


#include "../core/common_types.hpp"
#include "../ui/widget/text.hpp"


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
	void draw(drawbuffer* pBuffer);

	bool doMustDisappear();
};


#endif
