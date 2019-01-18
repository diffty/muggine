#include "or_game_mode.hpp"
#include "or_game_manager.hpp"
#include "rsc_manager.hpp"

#include <cmath>


ORGameMode* ORGameMode::s_pInstance = NULL;


ORGameMode::ORGameMode(Scene* pMainScene) {
    // Init shit
	ORGameMode::s_pInstance = this;

	m_pMainScene = pMainScene;

	m_bWinTimerActivated = false;
	m_bWaitForEnd = false;
	m_bLevelEnded = false;
    
    m_fHouse = 0.0;
    m_fIndustry = 0.0;
    m_fEcology = 0.0;
    m_fPopularity = 0.0;
    m_fPopularityGoal = 0.0;
    m_fPopulation = 0.0;
    m_fTomatoMalus = 0.0;

	m_fCarSpeed = CAR_SPEED;

	// TO DELETE
	m_fPopGrowth = 0.0;
    m_fIndGrowth = 0.0;
    
    m_fLastFrameIndustry = 0.0;
    m_fLastFrameHouse = 0.0;
    m_fLastFramePopulation = 0.0;
    // END TO DELETE

    initList(&m_llWidgetTrash);
    
    initScene();
    
    updateProgressBars();

	setTimer(TIME_LIMIT);

	m_bLevelEnded = false;

	/*m_pParticleSystem = new ParticleSystem;

	m_pParticleSystem->getRect()->setPos(150, 100);
	m_pParticleSystem->setParentWidget(pMainScene);
	m_pParticleSystem->setDrawOrder(6000);*/
    
    m_fTimeBeforeNextBuilding = 0.0;
    m_fTimeBeforeNextStatLoss = TIME_BETWEEN_STAT_LOSS;
}

ORGameMode::~ORGameMode() {
	m_pMainScene->garbageCollect();
	m_pMainScene->destroy();
}

void ORGameMode::initScene() {
	RscManager* rscManager = RscManager::get();
    
	vect2df_t bgSprPos;
	bgSprPos.x = 0;
	bgSprPos.y = 0;

    m_pBgSpr = new Sprite(RscManager::get()->getImgRsc(26), bgSprPos);
    
    m_carObj.translate(10, 150);
    m_cityObj.translate(0, 0);
    
    m_scrollingCloudsImg = new XScrollingImg(RscManager::get()->getImgRsc(25), 2.0);
    m_scrollingMountainsImg = new XScrollingImg(RscManager::get()->getImgRsc(24), 10.0);

    m_pMainScene->addComponent(m_pBgSpr);
    m_pMainScene->addComponent(m_scrollingCloudsImg);
    m_pMainScene->addComponent(&m_introAnimation);
    m_pMainScene->addComponent(m_scrollingMountainsImg);
    m_pMainScene->addComponent(&m_cityObj);
    m_pMainScene->addComponent(&m_roadObj);
    m_pMainScene->addComponent(&m_pickupItemsManager);
    m_pMainScene->addComponent(&m_carObj);
    
    // UI: Progress bars
    m_pHouseBar = new ProgressBar(8, 6, 96, 9);
    m_pHouseBar->setBorderColor(255, 255, 255);
    m_pHouseBar->setCompletedColor(73, 190, 247);
    m_pHouseBar->setRemainingColor(32, 85, 109);

    m_pIndustryBar = new ProgressBar(112, 6, 96, 9);
    m_pIndustryBar->setBorderColor(255, 255, 255);
    m_pIndustryBar->setCompletedColor(255, 85, 198);
    m_pIndustryBar->setRemainingColor(174, 40, 130);
    
    m_pEcologyBar = new ProgressBar(215, 6, 96, 9);
    m_pEcologyBar->setBorderColor(255, 255, 255);
    m_pEcologyBar->setCompletedColor(57, 243, 57);
    m_pEcologyBar->setRemainingColor(28, 125, 28);
    
    m_pPopularityBar = new ProgressBar(51, 214, 213, 12, -1.0, 1.0, 0.0);
    m_pPopularityBar->setBorderColor(255, 255, 255);
    m_pPopularityBar->setCompletedColor(251, 146, 77);
    m_pPopularityBar->setRemainingColor(170, 77, 16);
    
    // UI: Progress bars icons
    Sprite* pIndustryIcon = new Sprite(rscManager->getImgRsc(12), 105, 10);
    Sprite* pHouseIcon = new Sprite(rscManager->getImgRsc(11), 4, 9);
    Sprite* pEcologyIcon = new Sprite(rscManager->getImgRsc(10), 213, 9);
    Sprite* pPopularityIcon = new Sprite(rscManager->getImgRsc(17), 8, 198);
    Sprite* pPopularityLabel = new Sprite(rscManager->getImgRsc(29), 73, 229);

    // UI: Population
    Sprite* pPopulationLabel = new Sprite(rscManager->getImgRsc(33), 7, 31);
    m_pPopulationCountLabel = new Text("0", rscManager->getFontRsc(27), 7, 39);
    
    m_pMainScene->addComponent(m_pIndustryBar);
    m_pMainScene->addComponent(m_pHouseBar);
    m_pMainScene->addComponent(m_pEcologyBar);
    m_pMainScene->addComponent(m_pPopularityBar);
    m_pMainScene->addComponent(pPopularityLabel);
    m_pMainScene->addComponent(pPopulationLabel);
    m_pMainScene->addComponent(m_pPopulationCountLabel);
    
    m_pMainScene->addComponent(pIndustryIcon);
    m_pMainScene->addComponent(pHouseIcon);
    m_pMainScene->addComponent(pEcologyIcon);
    m_pMainScene->addComponent(pPopularityIcon);
    
    // UI: Timer
    vect2df_t fTimerPosition = {269, 218};
    m_pElectionTimerLabel = new Sprite(RscManager::get()->getImgRsc(28), 270, 211);
    m_pMinutesTimerLabel = new Text("00", RscManager::get()->getFontRsc(27), fTimerPosition.x, fTimerPosition.y);
    m_pSeparatorTimerLabel = new Text(":", RscManager::get()->getFontRsc(27), fTimerPosition.x + 21, fTimerPosition.y);
    m_pSecondsTimerLabel = new Text("00", RscManager::get()->getFontRsc(27), fTimerPosition.x + 25, fTimerPosition.y);
    
    m_pMainScene->addComponent(m_pMinutesTimerLabel);
    m_pMainScene->addComponent(m_pSeparatorTimerLabel);
    m_pMainScene->addComponent(m_pSecondsTimerLabel);
    m_pMainScene->addComponent(m_pElectionTimerLabel);
}

void ORGameMode::update() {
    Input* pInputSys = System::get()->getInputSys();
    double fDeltaTime = System::get()->getDeltaTime();

    vect2df_t vCurrPos = m_carObj.getRect()->getPos();
    
    // Inputs
    if ((pInputSys->IsKeyPressed(KEYB_Q) || pInputSys->IsKeyPressed(KEYB_A) || pInputSys->IsJoyBtnPressed(JOY_LEFT)) && vCurrPos.x > 0) {
        m_carObj.translate(-fDeltaTime * m_fCarSpeed, 0.0, TRANSFORM_REL);
        m_roadObj.setScrollSpeed(SCROLL_SPEED_BREAK);
        m_carObj.onBreaking();
        m_bCarWasMoving = true;
    }
    else if ((pInputSys->IsKeyPressed(KEYB_D) || pInputSys->IsJoyBtnPressed(JOY_RIGHT)) && vCurrPos.x < 320 - m_carObj.getRect()->getSize().w) {
        m_carObj.translate(fDeltaTime * m_fCarSpeed, 0.0, TRANSFORM_REL);
        m_roadObj.setScrollSpeed(SCROLL_SPEED_ACCELERATION);
        m_carObj.onAccelerating();
        m_bCarWasMoving = true;
    }
    else if (m_bCarWasMoving) {
        m_roadObj.setScrollSpeed(SCROLL_SPEED_NORMAL);
        m_carObj.onNormal();
        m_bCarWasMoving = false;
    }
    if ((pInputSys->IsKeyPressed(KEYB_Z) || pInputSys->IsKeyPressed(KEYB_W) || pInputSys->IsJoyBtnPressed(JOY_UP)) && vCurrPos.y > 136) {
        m_carObj.translate(0.0, -fDeltaTime * m_fCarSpeed, TRANSFORM_REL);
    }
    else if ((pInputSys->IsKeyPressed(KEYB_S) || pInputSys->IsJoyBtnPressed(JOY_DOWN)) && vCurrPos.y < 200 - m_carObj.getRect()->getSize().h) {
        m_carObj.translate(0.0, fDeltaTime * m_fCarSpeed, TRANSFORM_REL);
    }
    
    if (m_fTimeBeforeNextBuilding < 0.0) {
        int totalIndices = m_fHouse + m_fIndustry + m_fEcology;
        
        EBuildingType eNewBuildingType;
        
        if (totalIndices > 0) {
            int pickedCategory = System::get()->getRandInt(0, totalIndices);

            if (pickedCategory <= m_fHouse)
                eNewBuildingType = EBuildingType_House;
            else if (pickedCategory <= m_fIndustry + m_fHouse)
                eNewBuildingType = EBuildingType_Factory;
            else if (pickedCategory <= m_fIndustry + m_fHouse + m_fEcology)
                eNewBuildingType = EBuildingType_Tree;

            switch (eNewBuildingType) {
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
        }
        
        m_fTimeBeforeNextBuilding = ((float) System::get()->getRandInt(5, 7)) - minf(CITY_SIZE_SPAWN_LIMIT, m_cityObj.getCitySize()) / CITY_SIZE_SPAWN_LIMIT * RESPAWN_FREQ_COEF;
    }   
    else {
        m_fTimeBeforeNextBuilding -= System::get()->getDeltaTime();
    }
    
    m_fLastFrameIndustry = m_fIndustry;
    m_fLastFramePopulation = m_fPopulation;
    m_fLastFrameHouse = m_fHouse;

    // Collision detection (lol dez')
    Rectf* carCollisionRect = m_carObj.getCollisionRect();
    quad2df_t carCollisionRectQuad = carCollisionRect->getQuad2d();
    
    LLNode* pCurrNode = m_pickupItemsManager.getItemsList()->pHead;

    while (pCurrNode != NULL) {
        ORPickupItem* pCurrItem = (ORPickupItem*) pCurrNode->pData;
        pCurrNode = pCurrNode->pNext;
        
        Rectf* itemCollisionRect = pCurrItem->getRect();
        quad2df_t itemCollisionRectQuad = itemCollisionRect->getQuad2d();
        
        if (   itemCollisionRect->isPointInRect(carCollisionRectQuad.tr.x, carCollisionRectQuad.tr.y)
            || itemCollisionRect->isPointInRect(carCollisionRectQuad.br.x, carCollisionRectQuad.br.y)
            || itemCollisionRect->isPointInRect(carCollisionRectQuad.tl.x, carCollisionRectQuad.tl.y)
            || itemCollisionRect->isPointInRect(carCollisionRectQuad.bl.x, carCollisionRectQuad.bl.y)
            || carCollisionRect->isPointInRect(itemCollisionRectQuad.tr.x, itemCollisionRectQuad.tr.y)
            || carCollisionRect->isPointInRect(itemCollisionRectQuad.br.x, itemCollisionRectQuad.br.y)
            || carCollisionRect->isPointInRect(itemCollisionRectQuad.tl.x, itemCollisionRectQuad.tl.y)
            || carCollisionRect->isPointInRect(itemCollisionRectQuad.bl.x, itemCollisionRectQuad.bl.y)) {
            
            pCurrItem->onPickup();
        }
    }
    
#if DEBUG==1
    if (pInputSys->IsKeyPressed(KEYB_R)) {
        m_fHouse = maxf(0.0, m_fHouse - (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_T)) {
        m_fHouse = minf(100., m_fHouse + (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_F)) {
        m_fIndustry = maxf(0., m_fIndustry - (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_G)) {
        m_fIndustry = minf(100., m_fIndustry + (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_V)) {
        m_fEcology = maxf(0., m_fEcology - (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_B)) {
        m_fEcology = minf(100., m_fEcology + (fDeltaTime * 100.0));
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_W)) {
        m_fPopulation -= (fDeltaTime * 10.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_C)) {
        m_fPopulation += (fDeltaTime * 10.0);
        updateProgressBars();
    }
    if (pInputSys->IsKeyPressed(KEYB_E)) {
        m_carObj.showAdvice("MIOW");
    }
#endif
    
    updateGameStats();
    
    destroyWidgetsInTrash();
    
    if (m_fTimer < 0.0) {
        onTimerEnd();
    }
    else {
        m_fTimer -= System::get()->getDeltaTime();
        updateTimerLabel();
    }
}

void ORGameMode::debugDraw(uint8* buffer) {
    
}

void ORGameMode::updateGameStats() {
    float fDeltaTime = System::get()->getDeltaTime();
    
    // Population management
    m_fPopGrowth = ((m_fPopularity) / 100.0) * fDeltaTime * POPULATION_GROWTH_FACTOR;
    m_fPopGrowth *= (m_fHouse / 100.0);
    setPopulation(maxf(0.0, m_fPopulation + m_fPopGrowth));
    
    float fAllowedFailure = MIN_FAILURE_ALLOWED + (m_fEcology / 100.) * MAX_FAILURE_ALLOWED;
    float fFailFactor = 1. - minf(fabs((m_fHouse / 100.0) - (m_fIndustry / 100.)) * 4.0, 1.0);
    float fLowPopulationFactor = (1. * (minf(m_fPopulation / 100., 1.0)));
    
    //m_fPopularity = minf(1., (fAllowedFailure + fFailFactor)) * 200. - 100.;
    
    // Low population bonus gifted with early housing development
    m_fPopularityGoal = 100 * ((1. - fLowPopulationFactor) * (m_fHouse / 100.));
    
    // Classic popularity computation
    m_fPopularityGoal += minf(1., fAllowedFailure + fFailFactor) * (200. * fLowPopulationFactor) - (100. * fLowPopulationFactor);
    
    if (m_fTimeBeforeNextStatLoss < 0.) {
        m_fHouse = maxf(0., m_fHouse - HOUSE_LOSS_FACTOR);
        m_fIndustry = maxf(0., m_fIndustry - INDUSTRY_LOSS_FACTOR);
        m_fEcology = maxf(0., m_fEcology - ECOLOGY_LOSS_FACTOR);
        m_fTimeBeforeNextStatLoss = TIME_BETWEEN_STAT_LOSS;
    }
    else {
        m_fTimeBeforeNextStatLoss -= System::get()->getDeltaTime();
    }
    
    if (m_fTomatoMalus > 0.0) {
        m_fTomatoMalus -= TOMATO_RECOVER_SPEED * System::get()->getDeltaTime();
        if (m_fTomatoMalus <= 0.0) {
            m_fTomatoMalus = 0.0;
        }
    }
    
    // Apply malus
    m_fPopularityGoal -= m_fTomatoMalus;
    
    float fPopularityGoalDelta = m_fPopularityGoal - m_fPopularity;
    float fPopularityGoalDeltaSign = (fPopularityGoalDelta != 0) ? (fabs(fPopularityGoalDelta) / fPopularityGoalDelta) : 0;
    
    m_fPopularity += fPopularityGoalDeltaSign * System::get()->getDeltaTime() * POPULARITY_CHANGE_SPEED;
    
    
    /*
    printf("Population: %f (%f)\n", m_fPopulation, m_fPopGrowth);
    printf("Popularity: %f\n", m_fPopularity);
    printf("House: %f\n", m_fHouse);
    printf("Industry: %f (%f)\n", m_fIndustry, m_fIndGrowth);
    printf("Ecology: %f\n", m_fEcology);
    printf("City Size: %f\n", m_cityObj.getCitySize());
    printf("----\n");
     */
    
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
    
    if (m_fPopularityGoal - m_fTomatoMalus > -100.0) {
        m_fTomatoMalus += TOMATO_MALUS;
    }
    
    updateProgressBars();
}

void ORGameMode::onTimerEnd() {
    ORGameManager::get()->onEndLevel();
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

void ORGameMode::setTimer(float fTimer) {
	m_fTimer = fTimer;
    updateTimerLabel();
}

void ORGameMode::setPopulation(float fPopulation) {
    m_fPopulation = fPopulation;
    m_pPopulationCountLabel->setText((int) m_fPopulation);
}

void ORGameMode::updateTimerLabel() {
    int iCurrTime = (int) m_fTimer;
    
    int iMinutes = iCurrTime / 60;
    int iSeconds = iCurrTime % 60;
    
    m_pMinutesTimerLabel->setText(iMinutes, 2);
    m_pSecondsTimerLabel->setText(iSeconds, 2);
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
