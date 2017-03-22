#ifndef BKO_BALL_HPP
#define BKO_BALL_HPP

#include "sprite.hpp"


class Ball : public Sprite {
public:
	Ball(uint rscId, RscManager* pRscManager)
		: Sprite(rscId, pRscManager)
	{
		m_radius = m_rect.getSize().w;
	};

	vect2d_t getVelocity();
	void setVelocity(vect2d_t newVelocity);
	void setVelocity(uint x, uint y);
	void move();
	void update();

private:
	vect2d_t m_velocity;
	uint m_radius;
	bool m_bIsMoving = true;
};

#endif