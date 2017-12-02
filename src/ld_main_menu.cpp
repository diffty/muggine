#include "ld_main_menu.hpp"


LDMainMenu::LDMainMenu(Scene* pScene) {
	m_pScene = pScene;
	
	initScene();
}


LDMainMenu::~LDMainMenu() {
	m_pScene->destroy();
}


void LDMainMenu::initScene() {
	RscManager* pRscManager = RscManager::get();

	vect2df_t vNewSprPos;
	vNewSprPos.x = 60;
	vNewSprPos.y = 10;

	Sprite* pTitleSpr = new Sprite(pRscManager->getImgRsc(4), vNewSprPos);

	vNewSprPos.x = 0;
	vNewSprPos.y = 0;
	Sprite* pCreditsSpr = new Sprite(pRscManager->getImgRsc(5), vNewSprPos);

	vNewSprPos.x = 130;
	vNewSprPos.y = 110;
	m_pContinueBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(3), vNewSprPos, 5, 4, 0);
	showContinueBtn(false);

	vNewSprPos.y += 20;
	m_pNewBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(3), vNewSprPos, 1, 0, 0);

	vNewSprPos.y += 20;
	m_pQuitBtn = new ImageButtonWidget(pRscManager->getSprShtRsc(3), vNewSprPos, 3, 2, 0);

	m_pScene->addComponent(pTitleSpr);
	m_pScene->addComponent(pCreditsSpr);
	m_pScene->addComponent(m_pContinueBtn);
	m_pScene->addComponent(m_pNewBtn);
	m_pScene->addComponent(m_pQuitBtn);
}

void LDMainMenu::setContinueBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pContinueBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void LDMainMenu::setNewBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pNewBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void LDMainMenu::setQuitBtnCallback(void(*pCallback)(void*), void* pCallbackArg) {
	m_pQuitBtn->setOnReleaseCallback(pCallback, pCallbackArg);
}

void LDMainMenu::showContinueBtn(bool bIsBtnVisible) {
	m_pContinueBtn->setActive(bIsBtnVisible);
}