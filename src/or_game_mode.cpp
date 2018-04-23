#include "or_game_mode.hpp"
#include "or_game_manager.hpp"
#include "rsc_manager.hpp"


ORGameMode* ORGameMode::s_pInstance = NULL;


ORGameMode::ORGameMode(Scene* pMainScene) {
    // Init shit
	ORGameMode::s_pInstance = this;

	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;
    
    m_fHouse = 0;
    m_fIndustry = 0;
    m_fEcology = 0;
    m_fPopularity = 0.0;
    m_fPopulation = 0.0;

    initList(&m_llWidgetTrash);
    
    initScene();
    
    updateProgressBars();

	setTimer(11);

	m_bLevelEnded = false;

	m_pParticleSystem = new ParticleSystem;

	m_pParticleSystem->getRect()->setPos(150, 100);
	m_pParticleSystem->setParentWidget(pMainScene);
	m_pParticleSystem->setDrawOrder(6000);
    
    m_fTimeBeforeNextBuilding = 0.0;
}

ORGameMode::~ORGameMode() {
	m_pMainScene->garbageCollect();
	m_pMainScene->destroy();
}

void ORGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
    
    m_pBgSpr = new Sprite(RscManager::get()->getImgRsc(8), {0, 0});
    
    m_carObj.translate(10, 150);
    m_cityObj.translate(0, 0);
    
    m_pMainScene->addComponent(m_pBgSpr);
    m_pMainScene->addComponent(&m_cityObj);
    m_pMainScene->addComponent(&m_roadObj);
    m_pMainScene->addComponent(&m_pickupItemsManager);
    m_pMainScene->addComponent(&m_carObj);
    
    // UI: Progress bars
    m_pHouseBar = new ProgressBar({8, 6}, {96, 9});
    m_pHouseBar->setBorderColor(255, 255, 255);
    m_pHouseBar->setCompletedColor(73, 190, 247);
    m_pHouseBar->setRemainingColor(32, 85, 109);

    m_pIndustryBar = new ProgressBar({112, 6}, {96, 9});
    m_pIndustryBar->setBorderColor(255, 255, 255);
    m_pIndustryBar->setCompletedColor(255, 85, 198);
    m_pIndustryBar->setRemainingColor(174, 40, 130);
    
    m_pEcologyBar = new ProgressBar({215, 6}, {96, 9});
    m_pEcologyBar->setBorderColor(255, 255, 255);
    m_pEcologyBar->setCompletedColor(57, 243, 57);
    m_pEcologyBar->setRemainingColor(28, 125, 28);
    
    m_pPopularityBar = new ProgressBar({51, 214}, {213, 12}, -1.0, 1.0, 0.0);
    m_pPopularityBar->setBorderColor(255, 255, 255);
    m_pPopularityBar->setCompletedColor(251, 146, 77);
    m_pPopularityBar->setRemainingColor(170, 77, 16);
    
    // UI ; Progress bars icons
    Sprite* pIndustryIcon = new Sprite(rscManager->getImgRsc(12), {105, 6});
    Sprite* pHouseIcon = new Sprite(rscManager->getImgRsc(11), {4, 9});
    Sprite* pEcologyIcon = new Sprite(rscManager->getImgRsc(10), {213, 9});
    Sprite* pPopularityIcon = new Sprite(rscManager->getImgRsc(17), {8, 198});
    
    m_pMainScene->addComponent(m_pIndustryBar);
    m_pMainScene->addComponent(m_pHouseBar);
    m_pMainScene->addComponent(m_pEcologyBar);
    m_pMainScene->addComponent(m_pPopularityBar);
    
    m_pMainScene->addComponent(pIndustryIcon);
    m_pMainScene->addComponent(pHouseIcon);
    m_pMainScene->addComponent(pEcologyIcon);
    m_pMainScene->addComponent(pPopularityIcon);
}

void ORGameMode::update() {
    Input* pInputSys = System::get()->getInputSys();
    double fDeltaTime = System::get()->getDeltaTime();

    vect2df_t vCurrPos = m_carObj.getRect()->getPos();
    
    // Inputs
    if (pInputSys->IsKeyPressed(KEYB_Q) && vCurrPos.x > 0) {
        m_carObj.translate(-fDeltaTime * m_fCarSpeed, 0.0, TRANSFORM_REL);
    }
    else if (pInputSys->IsKeyPressed(KEYB_D) && vCurrPos.x < 320 - m_carObj.getRect()->getSize().w) {
        m_carObj.translate(fDeltaTime * m_fCarSpeed, 0.0, TRANSFORM_REL);
    }
    if (pInputSys->IsKeyPressed(KEYB_Z) && vCurrPos.y > 136) {
        m_carObj.translate(0.0, -fDeltaTime * m_fCarSpeed, TRANSFORM_REL);
    }
    else if (pInputSys->IsKeyPressed(KEYB_S) && vCurrPos.y < 200 - m_carObj.getRect()->getSize().h) {
        m_carObj.translate(0.0, fDeltaTime * m_fCarSpeed, TRANSFORM_REL);
    }
    
    if (m_fTimeBeforeNextBuilding < 0.0) {
        switch ((EBuildingType) System::get()->getRandInt(0, 4)) {
            case EBuildingType_Factory:
                if (m_fIndustry > 1.0) {
                    m_cityObj.generateNewBuilding(EBuildingType_Factory);
                }
                break;
                
            case EBuildingType_House:
                if (m_fPopulation > 1.0) {
                    m_cityObj.generateNewBuilding(EBuildingType_House);
                }
                break;
            
            case EBuildingType_Tree:
                if (m_fEcology >= 1.0) {
                    m_cityObj.generateNewBuilding(EBuildingType_Tree);
                }
                break;
        }
        
        m_fTimeBeforeNextBuilding = ((float) System::get()->getRandInt(5, 7)) * minf(50.0, m_cityObj.getCitySize()) / 50.0 * 0.7;
    }   
    else {
        m_fTimeBeforeNextBuilding -= System::get()->getDeltaTime();
    }
    
    m_fLastFrameIndustry = m_fIndustry;
    m_fLastFramePopulation = m_fPopulation;
    m_fLastFrameHouse = m_fHouse;

    // Collision detection (lol dez')
    Rectf* vCarRect = m_carObj.getRect();
    
    vect2df_t vCarTopLeft;
    vCarTopLeft.x = vCarRect->getPos().x;
    vCarTopLeft.y = vCarRect->getPos().y;
    
    vect2df_t vCarTopRight;
    vCarTopRight.x = vCarRect->getPos().x + vCarRect->getSize().w;
    vCarTopRight.y = vCarRect->getPos().y;
    
    vect2df_t vCarBottomLeft;
    vCarBottomLeft.x = vCarRect->getPos().x;
    vCarBottomLeft.y = vCarRect->getPos().y + vCarRect->getSize().h;
    
    vect2df_t vCarBottomRight;
    vCarBottomRight.x = vCarRect->getPos().x + vCarRect->getSize().w;
    vCarBottomRight.y = vCarRect->getPos().y + vCarRect->getSize().h;
    
    LLNode* pCurrNode = m_pickupItemsManager.getItemsList()->pHead;

    while (pCurrNode != NULL) {
        ORPickupItem* pCurrItem = (ORPickupItem*) pCurrNode->pData;
        pCurrNode = pCurrNode->pNext;
        
        Rectf* vItemRect = pCurrItem->getRect();
        
        vect2df_t vItemTopLeft;
        vItemTopLeft.x = vItemRect->getPos().x;
        vItemTopLeft.y = vItemRect->getPos().y;
        
        vect2df_t vItemTopRight;
        vItemTopRight.x = vItemRect->getPos().x + vItemRect->getSize().w;
        vItemTopRight.y = vItemRect->getPos().y;
        
        vect2df_t vItemBottomLeft;
        vItemBottomLeft.x = vItemRect->getPos().x;
        vItemBottomLeft.y = vItemRect->getPos().y + vItemRect->getSize().h;
        
        vect2df_t vItemBottomRight;
        vItemBottomRight.x = vItemRect->getPos().x + vItemRect->getSize().w;
        vItemBottomRight.y = vItemRect->getPos().y + vItemRect->getSize().h;
        
        if (   pCurrItem->getRect()->isPointInRect(vCarTopRight.x, vCarTopRight.y)
            || pCurrItem->getRect()->isPointInRect(vCarBottomRight.x, vCarBottomRight.y)
            || pCurrItem->getRect()->isPointInRect(vCarTopLeft.x, vCarTopLeft.y)
            || pCurrItem->getRect()->isPointInRect(vCarBottomLeft.x, vCarBottomLeft.y)
            || m_carObj.getRect()->isPointInRect(vItemTopRight.x, vItemTopRight.y)
            || m_carObj.getRect()->isPointInRect(vItemBottomRight.x, vItemBottomRight.y)
            || m_carObj.getRect()->isPointInRect(vItemTopLeft.x, vItemTopLeft.y)
            || m_carObj.getRect()->isPointInRect(vItemBottomLeft.x, vItemBottomLeft.y)) {
            
            pCurrItem->onPickup();
        }
    }
    
#if DEBUG==1
    if (pInputSys->IsKeyPressed(KEYB_R)) {
        m_fHouse -= (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_T)) {
        m_fHouse += (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_F)) {
        m_fIndustry -= (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_G)) {
        m_fIndustry += (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_V)) {
        m_fEcology -= (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_B)) {
        m_fEcology += (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_W)) {
        m_fPopularity -= (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_C)) {
        m_fPopularity += (fDeltaTime * 100.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_E)) {
        m_carObj.showAdvice("MIOW");
    }
#endif
    
    updateGameStats();
    
    destroyWidgetsInTrash();
}

void ORGameMode::updateGameStats() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    // Population management
    m_fPopGrowth = ((m_fPopularity) / 100.0) * fDeltaTime * 5.0;
    m_fPopGrowth *= (m_fHouse / 100.0);
    m_fPopulation = maxf(0.0, m_fPopulation + m_fPopGrowth);
    
    // Industry management
    m_fIndGrowth = minf(0.0, m_fHouse - m_fIndustry) * fDeltaTime * (0.001 * m_fPopulation);
    m_fIndustry = maxf(0.0, m_fIndustry + m_fIndGrowth);
    
    printf("Population: %f (%f)\n", m_fPopulation, m_fPopGrowth);
    printf("Popularity: %f\n", m_fPopularity);
    printf("House: %f\n", m_fHouse);
    printf("Industry: %f (%f)\n", m_fIndustry, m_fIndGrowth);
    printf("Ecology: %f\n", m_fEcology);
    printf("City Size: %f\n", m_cityObj.getCitySize());
    printf("----\n");
    
    // Popularity management
    m_fPopularity += maxf(0.0, m_fHouse - m_fLastFrameHouse);
    if (m_fLastFrameHouse - m_fHouse > 0.0) {
        printf("%f\n", m_fLastFrameHouse - m_fHouse);
    }
    
    updateProgressBars();
}

void ORGameMode::onHouseItemPicked(ORPickupItem* pPickedUpItem) {
    m_pickupItemsManager.onItemPicked(pPickedUpItem);
    m_fHouse = minf(m_fHouse + 2.0, 100.0);
    updateProgressBars();
}

void ORGameMode::onIndustryItemPicked(ORPickupItem* pPickedUpItem) {
    m_pickupItemsManager.onItemPicked(pPickedUpItem);
    m_fIndustry = minf(m_fIndustry + 2.0, 100.0);
    updateProgressBars();
}

void ORGameMode::onEcologyItemPicked(ORPickupItem* pPickedUpItem) {
    m_pickupItemsManager.onItemPicked(pPickedUpItem);
    m_fEcology = minf(m_fEcology + 2.0, 100.0);
    updateProgressBars();
}

void ORGameMode::onTomatoItemPicked(ORPickupItem* pPickedUpItem) {
    m_pickupItemsManager.onItemPicked(pPickedUpItem);
    m_fPopularity = maxf(m_fPopularity - 3.0, -100.0);
    updateProgressBars();
}

void ORGameMode::updateProgressBars() {
    m_pHouseBar->setValue(maxf(0.0, minf(1.0, (float) m_fHouse / 100.0)));
    m_pIndustryBar->setValue(maxf(0.0, minf(1.0, (float) m_fIndustry / 100.0)));
    m_pEcologyBar->setValue(maxf(0.0, minf(1.0, (float) m_fEcology / 100.0)));
    m_pPopularityBar->setValue(maxf(-1.0, minf(1.0, (((float) (100.0 + m_fPopularity) / 200.0) - 0.5) * 2.0)));
}


Scene* ORGameMode::getScene() {
	return m_pMainScene;
}

float ORGameMode::getTimer() {
	return m_fTimer;
}

float ORGameMode::getRoadScrollSpeed() {
    return m_roadObj.getScrollSpeed();
}

float ORGameMode::getIndustryValue() {
    return m_fIndustry;
}

float ORGameMode::getEcologyValue() {
    return m_fEcology;
}

float ORGameMode::getHouseValue() {
    return m_fHouse;
}

float ORGameMode::getPopularityValue() {
    return m_fPopularity;
}

float ORGameMode::getPopulationValue() {
    return m_fPopulation;
}

float ORGameMode::getPopulationGrowth() {
    return m_fPopGrowth;
}

float ORGameMode::getIndustryGrowth() {
    return m_fIndGrowth;
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

void ORGameMode::destroyWidget(IWidget* pWidget) {
    addDataToList(&m_llWidgetTrash, pWidget);
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
