#include "ts_main_menu.hpp"


TSMainMenu::TSMainMenu(Scene* pScene) {
	m_pScene = pScene;
	
	initScene();
}


TSMainMenu::~TSMainMenu() {
	m_pScene->destroy();
}


void TSMainMenu::initScene() {
	RscManager* pRscManager = RscManager::get();

	vect2df_t vNewSprPos;
	vNewSprPos.x = 60;
	vNewSprPos.y = 10;

	Sprite* pTitleSpr = new Sprite(pRscManager->getImgRsc(10), vNewSprPos);

	vNewSprPos.x = 0;
	vNewSprPos.y = 0;
	Sprite* pCreditsSpr = new Sprite(pRscManager->getImgRsc(14), vNewSprPos);

	vNewSprPos.x = 130;
	vNewSprPos.y = 110;
	m_pContinueBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(11), vNewSprPos, 5, 4, 0);
	showContinueBtn(false);

	vNewSprPos.y += 20;
	m_pNewBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(11), vNewSprPos, 1, 0, 0);

	vNewSprPos.y += 20;
	m_pQuitBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(11), vNewSprPos, 3, 2, 0);

	m_pScene->addComponent(pTitleSpr);
	m_pScene->addComponent(pCreditsSpr);
	m_pScene->addComponent(m_pContinueBtn);
	m_pScene->addComponent(m_pNewBtn);
	m_pScene->addComponent(m_pQuitBtn);
}

void TSMainMenu::setContinueBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pContinueBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void TSMainMenu::setNewBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pNewBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void TSMainMenu::setQuitBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pQuitBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void TSMainMenu::showContinueBtn(bool bIsBtnVisible) {
	m_pContinueBtn->setActive(bIsBtnVisible);
}