#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"


class LDLevelSuccessScreen
{
private:
	Text* m_pLevelTextLabel;
	//Text* m_pLevelNumTextLabel;

	Text* m_pMoneyMadeLabel;
	Text* m_pMoneyMadeValue;

	Text* m_pHealthRemainingLabel;
	Text* m_pHealthRemainingValue;

	Text* m_pTimeRemainingLabel;
	Text* m_pTimeRemainingValue;

	Text* m_pTotalScoreLabel;
	Text* m_pTotalScoreValue;

	float m_fTimeBeforeStateEnd = 5;

	float m_fMoneyScoreCount = 0;
	float m_fHealthScoreCount = 0;
	float m_fTimeScoreCount = 0;

	bool bTimerActive = false;
	bool bDoMustDisappear = false;
	int iIsMouseBtnState = 0;

	int m_iState;

public:
	LDLevelSuccessScreen(int iLevelNum);
	~LDLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

	void onNewState();
	void onEndState();

	bool doMustDisappear();
};


#endif
