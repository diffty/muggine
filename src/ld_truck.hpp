#ifndef LD_TRUCK_HPP
#define LD_TRUCK_HPP

#include "iwidget.hpp"
#include "spritesheet.hpp"
#include "ld_truck_content.hpp"


class Truck : public IWidget
{
private:
	SpriteSheet* m_pTruckSprSht;
	SpriteSheet* m_pWheelsSprSht;

	TruckContent* m_pContent;
	
	int m_iNbSlotW;
	int m_iNbSlotH;

	int m_iLoadAnimOffY = 0;

	float m_fLoadAnim = 0;
	bool m_bDoLoadAnim = false;

public:
	Truck(vect2df_t vPos, int iNbSlotW, int iNbSlotH);
	~Truck();

	void doLoadAnim();

	TruckContent* getContent();

	void updateContentPosition();

	void update();
	void draw(uint8* pBuffer);
};


#endif
