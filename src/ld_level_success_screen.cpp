#include "ld_level_success_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"
#include "ld_game_mode.hpp"


LDLevelSuccessScreen::LDLevelSuccessScreen(int iLevelNum) {
	m_iState = 0;

	m_pLevelTextLabel		= new Text("LEVEL CLEARED",		RscManager::get()->getFontRsc(0), { 100, 80 });
	//m_pLevelNumTextLabel	= new Text(iLevelNum,			RscManager::get()->getFontRsc(0), { 140, 80 });

	m_pMoneyMadeLabel		= new Text("MONEY MADE",		RscManager::get()->getFontRsc(0), { 80, 110 });
	m_pMoneyMadeValue		= new Text("",					RscManager::get()->getFontRsc(0), { 220, 110 });

	m_pHealthRemainingLabel	= new Text("MORAL REMAINING",	RscManager::get()->getFontRsc(0), { 80, 125 });
	m_pHealthRemainingValue	= new Text("",					RscManager::get()->getFontRsc(0), { 220, 125 });

	m_pTimeRemainingLabel	= new Text("TIME REMAINING",	RscManager::get()->getFontRsc(0), { 80, 140 });
	m_pTimeRemainingValue	= new Text("",					RscManager::get()->getFontRsc(0), { 220, 140 });

	m_pTotalScoreLabel		= new Text("TOTAL",				RscManager::get()->getFontRsc(0), { 80, 160 });
	m_pTotalScoreValue		= new Text("",					RscManager::get()->getFontRsc(0), { 220, 160 });

	m_pUpgradeButton = new ImageButtonWidget(
		RscManager::get()->getSprShtRsc(3),
		{ 120 , 200 },
		6,
		7,
		0,
		NULL,
		NULL
	);
	onNewState();

	m_bDoMustDisappear = false;
}


LDLevelSuccessScreen::~LDLevelSuccessScreen() {
	delete m_pLevelTextLabel;
	//delete m_pLevelNumTextLabel;

	delete m_pMoneyMadeLabel;
	delete m_pMoneyMadeValue;

	delete m_pHealthRemainingLabel;
	delete m_pHealthRemainingValue;

	delete m_pTimeRemainingLabel;
	delete m_pTimeRemainingValue;

	delete m_pTotalScoreLabel;
	delete m_pTotalScoreValue;
}

void LDLevelSuccessScreen::update() {
	if (m_iIsMouseBtnState == 0 && System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 1;
	}
	else if (m_iIsMouseBtnState == 1 && System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 2;
	}
	else if (m_iIsMouseBtnState > 0 && !System::get()->getInputSys()->IsButtonPressed(MOUSE_BTN_LEFT)) {
		m_iIsMouseBtnState = 0;
	}

	if (m_iIsMouseBtnState == 1) {
		onEndState();
	}

	switch (m_iState) {
	case 1:
		m_fMoneyScoreCount += System::get()->getDeltaTime() * 50;
		if (m_fMoneyScoreCount >= LDGameMode::get()->getMoney()) {
			onEndState();
		}
		else {
			m_pMoneyMadeValue->setText((int)m_fMoneyScoreCount);
			m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		}
		break;

	case 3:
		m_fHealthScoreCount += System::get()->getDeltaTime() * 50;
		if (m_fHealthScoreCount >= LDGameMode::get()->getHealth()) {
			onEndState();
		}
		else {
			m_pHealthRemainingValue->setText((int)m_fHealthScoreCount);
			m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		}
		break;

	case 5:
		m_fTimeScoreCount += System::get()->getDeltaTime() * 50;
		if (m_fTimeScoreCount >= LDGameMode::get()->getTimer()) {
			onEndState();
		}
		else {
			m_pTimeRemainingValue->setText((int)m_fTimeScoreCount);
			m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		}
		break;
	}

	if (m_bTimerActive && m_fTimeBeforeStateEnd > 0) {
		m_fTimeBeforeStateEnd -= System::get()->getDeltaTime();

		if (m_fTimeBeforeStateEnd <= 0) {
			onEndState();
		}
	}
}

void LDLevelSuccessScreen::onNewState() {
	m_fTimeBeforeStateEnd = -1;

	switch (m_iState) {
	case 0:
		m_fTimeBeforeStateEnd = 2;
		m_bTimerActive = true;
		break;

	case 1:
	case 3:
	case 5:
		m_bTimerActive = false;
		break;

	case 2:
	case 4:
		m_fTimeBeforeStateEnd = 1;
		m_bTimerActive = true;
		break;

	case 6:
		m_fTimeBeforeStateEnd = 2;
		m_bTimerActive = true;
		break;
	}
}

void LDLevelSuccessScreen::onEndState() {
	switch (m_iState) {
	case 1:
		m_fMoneyScoreCount = LDGameMode::get()->getMoney();
		m_pMoneyMadeValue->setText((int)m_fMoneyScoreCount);
		m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		break;

	case 3:
		m_fHealthScoreCount = LDGameMode::get()->getHealth();
		m_pHealthRemainingValue->setText((int)m_fHealthScoreCount);
		m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		break;

	case 5:
		m_fTimeScoreCount = LDGameMode::get()->getTimer();
		m_pTimeRemainingValue->setText((int)m_fTimeScoreCount);
		m_pTotalScoreValue->setText((int)m_fHealthScoreCount + m_fMoneyScoreCount + m_fTimeScoreCount);
		break;

	case 7:
		m_bDoMustDisappear = true;
	}

	m_iState++;
	onNewState();
}

void LDLevelSuccessScreen::draw(uint8* fb) {
	m_pLevelTextLabel->draw(fb);
	//m_pLevelNumTextLabel->draw(fb);

	m_pMoneyMadeLabel->draw(fb);
	m_pMoneyMadeValue->draw(fb);

	m_pHealthRemainingLabel->draw(fb);
	m_pHealthRemainingValue->draw(fb);

	m_pTimeRemainingLabel->draw(fb);
	m_pTimeRemainingValue->draw(fb);

	m_pTotalScoreLabel->draw(fb);
	m_pTotalScoreValue->draw(fb);
}

void LDLevelSuccessScreen::onUpgradeButtonPressed(void* args) {
	//GameManager::
}

bool LDLevelSuccessScreen::doMustDisappear() {
	return m_bDoMustDisappear;
}
