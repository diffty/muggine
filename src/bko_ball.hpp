#ifndef BKO_BALL_HPP
#define BKO_BALL_HPP

#include "sprite.hpp"
#include "system.hpp"


class Ball : public Sprite {
public:
	Ball(uint rscId, System* sys, RscManager* pRscManager)
		: Sprite(rscId, pRscManager)
	{
		m_radius = m_rect.getSize().w;
		m_sys = sys;
	};

	vect2d_t getVelocity();
	vect2d_t getCenterPos();
	void setVelocity(vect2d_t newVelocity);
	void setVelocity(uint x, uint y);
	void move();
	void update();

private:
	vect2d_t m_velocity;
	uint m_radius;
	bool m_bIsMoving = true;
	System* m_sys;
};

#endif