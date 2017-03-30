#ifndef BKO_BALL_HPP
#define BKO_BALL_HPP

#include "sprite.hpp"
#include "system.hpp"
#include "bko_grid.hpp"
#include "bko_paddle.hpp"


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
	void setVelocity(long x, long y);
	void move();
	void update();
	bool checkCollisionBetweenPos(Grid* pGrid, Paddle* pPaddle, vect2d_t currBallCenter, vect2d_t nextBallCenter, uint* pCollidingBrickId, vect2d_t* pCollisionPoint, int* pCollisionType);

private:
	vect2d_t m_velocity;
	uint m_radius;
	bool m_bIsMoving = true;
	System* m_sys;
};

#endif
