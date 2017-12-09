#include "ld_truck.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"

#define TRUCK_FRONT_SIZE 32
#define TRUCK_SLOT_SIZE 27
#define TRUCK_BACK_SIZE 37
#define TRUCK_WHEEL_FRONT_OFFX 4
#define TRUCK_WHEEL_FRONT_OFFY 48
#define TRUCK_WHEEL_BACK_OFFX 12
#define TRUCK_WHEEL_BACK_OFFY 48
#define TRUCK_CONTENT_OFFY 22
#define TRUCK_SLOT_SIZE_X 33
#define TRUCK_SLOT_SIZE_Y 11

#define TRUCK_BACK_TO_MID_X 12
#define TRUCK_BACK_TO_MID_Y -12

#define TRUCK_MID_TO_MID_X 12
#define TRUCK_MID_TO_MID_Y -12

#define TRUCK_MID_TO_FRONT_X 36
#define TRUCK_MID_TO_FRONT_Y 11

#define TRUCK_BACK_TO_FRONT_X 36
#define TRUCK_BACK_TO_FRONT_Y 10


Truck::Truck(vect2df_t vPos, int iNbSlotW, int iNbSlotH) :
	IWidget(vPos.x, vPos.y) {

	m_pTruckSprSht = RscManager::get()->getSprShtRsc(12);
	m_pWheelsSprSht = RscManager::get()->getSprShtRsc(13);

	m_iNbSlotW = iNbSlotW;
	m_iNbSlotH = iNbSlotH;
        
	m_pContent = new TruckContent(
		{
			0,
			0
		},
		{
			iNbSlotW * TRUCK_SLOT_SIZE_X,
			iNbSlotH * TRUCK_SLOT_SIZE_Y
		},
		iNbSlotW,
		iNbSlotH
	);

	updateContentPosition();
}


Truck::~Truck() {
	delete m_pContent;
}

void Truck::doLoadAnim() {
	m_bDoLoadAnim = true;
	m_fLoadAnim = 0;
}

TruckContent* Truck::getContent() {
	return m_pContent;
}

void Truck::updateContentPosition() {
	/*m_pContent->translate(
		getRect()->getPos().x + TRUCK_FRONT_SIZE,
		getRect()->getPos().y + TRUCK_CONTENT_OFFY + m_iLoadAnimOffY,
		TRANSFORM_ABS
	);*/

	m_pContent->translate(
		getRect()->getPos().x,
		getRect()->getPos().y + m_iLoadAnimOffY,
		TRANSFORM_ABS
	);
}

void Truck::update() {
	IWidget::update();

	if (m_bDoLoadAnim) {
		m_fLoadAnim += System::get()->getDeltaTime() * 8.;
		m_iLoadAnimOffY = ((1. - fabs(m_fLoadAnim - 1.)) * 2.);

		updateContentPosition();

		if (m_fLoadAnim >= 2.) {
			m_fLoadAnim = 0.;
			m_bDoLoadAnim = false;
		}
	}
}

void Truck::draw(uint8* pBuffer) {
	vect2df_t vTruckPos = getRect()->getPos();

	// printf("%d, %f\n", loadAnimOffY, m_fLoadAnim);
    
	int iCurX = vTruckPos.x;
	int iCurY = vTruckPos.y;

	//m_pWheelsSprSht->draw(pBuffer, 0, iCurX + TRUCK_WHEEL_FRONT_OFFX, vTruckPos.y + TRUCK_WHEEL_FRONT_OFFY, false, true);

	m_pTruckSprSht->draw(pBuffer, 2, iCurX, iCurY + m_iLoadAnimOffY, false, true);

	if (m_iNbSlotH > 1) {
		iCurX -= TRUCK_BACK_TO_MID_X;
		iCurY -= TRUCK_BACK_TO_MID_Y;
	}
	else {
		iCurX -= TRUCK_BACK_TO_FRONT_X;
		iCurY -= TRUCK_BACK_TO_FRONT_Y;
	}
	
	for (int i = 1; i < m_iNbSlotH; i++) {
		m_pTruckSprSht->draw(pBuffer, 1, iCurX, iCurY + m_iLoadAnimOffY, false, true);
		if (i == m_iNbSlotH - 1) {
			iCurX -= TRUCK_MID_TO_FRONT_X;
			iCurY -= TRUCK_MID_TO_FRONT_Y;
		}
		else {
			iCurX -= TRUCK_MID_TO_MID_X;
			iCurY -= TRUCK_MID_TO_MID_Y;
		}
	}

	//m_pWheelsSprSht->draw(pBuffer, 0, iCurX + TRUCK_WHEEL_BACK_OFFX, vTruckPos.y + TRUCK_WHEEL_BACK_OFFY, false, true);

	m_pContent->draw(pBuffer);

	m_pTruckSprSht->draw(pBuffer, 0, iCurX, iCurY + m_iLoadAnimOffY, false, true);

	
}
