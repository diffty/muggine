#ifndef LD_LEVEL_SUCCESS_SCREEN
#define LD_LEVEL_SUCCESS_SCREEN

#include "common_types.hpp"
#include "text.hpp"
#include "image_button_widget.hpp"


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

	ImageButtonWidget* m_pUpgradeButton;

	float m_fTimeBeforeStateEnd = 5;

	float m_fMoneyScoreCount = 0;
	float m_fHealthScoreCount = 0;
	float m_fTimeScoreCount = 0;

	bool m_bTimerActive = false;
	bool m_bDoMustDisappear = false;

	int m_iIsMouseBtnState = 0;
	int m_iState;

public:
	LDLevelSuccessScreen(int iLevelNum);
	~LDLevelSuccessScreen();

	void update();
	void draw(uint8* fb);

	void onNewState();
	void onEndState();

	static void onUpgradeButtonPressed(void* args);

	bool doMustDisappear();
};


#endif
