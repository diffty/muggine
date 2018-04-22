#include "or_game_mode.hpp"
#include "or_game_manager.hpp"
#include "rsc_manager.hpp"


ORGameMode* ORGameMode::s_pInstance = NULL;


ORGameMode::ORGameMode(Scene* pMainScene) :
    m_bgSpr(RscManager::get()->getImgRsc(8), {0, 0})
{
    // Init shit
	ORGameMode::s_pInstance = this;

	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;
    
	initScene();

	setTimer(11);

	initList(&m_llWidgetTrash);

	m_bLevelEnded = false;

	m_pParticleSystem = new ParticleSystem;

	m_pParticleSystem->getRect()->setPos(150, 100);
	m_pParticleSystem->setParentWidget(pMainScene);
	m_pParticleSystem->setDrawOrder(6000);
    
}

ORGameMode::~ORGameMode() {
	m_pMainScene->garbageCollect();
	m_pMainScene->destroy();
}

void ORGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
    
    m_carObj.translate(10, 150);
    
    m_pMainScene->addComponent(&m_bgSpr);
    m_pMainScene->addComponent(&m_roadObj);
    m_pMainScene->addComponent(&m_carObj);
    
    ORPickupItemHouse* pPickupObj = new ORPickupItemHouse();
    pPickupObj->translate(180, 160);
    m_pMainScene->addComponent(pPickupObj);
}

void ORGameMode::update() {
    Input* pInputSys = System::get()->getInputSys();
    double fDeltaTime = System::get()->getDeltaTime();
    
    float MOVE_SPEED = 50.0;
    
    vect2df_t vCurrPos = m_carObj.getRect()->getPos();
    
    if (pInputSys->IsKeyPressed(KEYB_Q) && vCurrPos.x > 0) {
        m_carObj.translate(-fDeltaTime * MOVE_SPEED, 0.0, TRANSFORM_REL);
    }
    else if (pInputSys->IsKeyPressed(KEYB_D) && vCurrPos.x < 320 - m_carObj.getRect()->getSize().w) {
        m_carObj.translate(fDeltaTime * MOVE_SPEED, 0.0, TRANSFORM_REL);
    }
    if (pInputSys->IsKeyPressed(KEYB_Z) && vCurrPos.y > 136) {
        m_carObj.translate(0.0, -fDeltaTime * MOVE_SPEED, TRANSFORM_REL);
    }
    else if (pInputSys->IsKeyPressed(KEYB_S) && vCurrPos.y < 170 - m_carObj.getRect()->getSize().h) {
        m_carObj.translate(0.0, fDeltaTime * MOVE_SPEED, TRANSFORM_REL);
    }
}

Scene* ORGameMode::getScene() {
	return m_pMainScene;
}

int ORGameMode::getMoney() {
	return m_iMoney;
}

float ORGameMode::getTimer() {
	return m_fTimer;
}

void ORGameMode::increaseMoney(int iMoneyBonus) {
	setMoney(m_iMoney + iMoneyBonus);
}

void ORGameMode::decreaseMoney(int iMoneyMalus) {
	setMoney(m_iMoney - iMoneyMalus);
}

void ORGameMode::setMoney(int iMoney) {
	m_iMoney = iMoney;
}

void ORGameMode::setTimer(float fTimer) {
	m_fTimer = fTimer;
}

void ORGameMode::destroyWidgetsInTrash() {
	LLNode* pCurrNode = m_llWidgetTrash.pHead;

	while (pCurrNode != NULL) {
		IWidget* pCurrCard = (IWidget*)pCurrNode->pData;
		delete pCurrCard;
		pCurrNode = pCurrNode->pNext;
	}

	clearList(&m_llWidgetTrash);
}

ORGameMode* ORGameMode::get() {
	return ORGameMode::s_pInstance;
}

void ORGameMode::initGameMode(ORGameMode** pGameMode, Scene* pScene) {
	if (*pGameMode) {
		//(*pGameMode)->getTruckContent()->unlinkAllWidgets();
		delete *pGameMode;
	}

	*pGameMode = new ORGameMode(pScene);
}
