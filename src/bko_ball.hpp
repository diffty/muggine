#ifndef BKO_BALL_HPP
#define BKO_BALL_HPP

#include "sprite.hpp"
#include "system.hpp"
#include "bko_grid.hpp"
#include "bko_paddle.hpp"
#include <cmath>


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
	void reinit(Paddle* pPaddle);
	bool checkCollisionBetweenPos(Grid* pGrid, Paddle* pPaddle, vect2d_t currBallCenter, vect2d_t nextBallCenter, uint* pCollidingBrickId, vect2d_t* pCollisionPoint, int* pCollisionType);
	
	void setIsDead(bool bIsDead) { m_bIsDead = bIsDead; }
	void setIsMoving(bool bIsMoving) { m_bIsMoving = bIsMoving; }
	void setIsStickToPaddle(bool bIsStickToPaddle) { m_bIsStickToPaddle = bIsStickToPaddle; }

	bool isDead() { return m_bIsDead; }
	bool isMoving() { return m_bIsMoving; }
	bool isStickToPaddle() { return m_bIsStickToPaddle; }

private:
	vect2d_t m_velocity;
	uint m_radius;
	System* m_sys;
	bool m_bIsMoving = false;
	bool m_bIsDead = false;
	bool m_bIsStickToPaddle = false;
};

#endif
