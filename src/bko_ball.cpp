#include "bko_ball.hpp"


void Ball::setVelocity(vect2d_t newVelocity) {
	m_velocity = newVelocity;
}

void Ball::setVelocity(long x, long y) {
	m_velocity.x = (long) x;
	m_velocity.y = (long) y;
}

vect2d_t Ball::getVelocity() {
	return m_velocity;
}

vect2d_t Ball::getCenterPos() {
	vect2d_t centerPos = m_rect.getPos();
	centerPos.x += m_rect.getSize().w / 2;
	centerPos.y += m_rect.getSize().h / 2;
	return centerPos;
}

void Ball::update() {
	if (m_bIsMoving) {
		// move();
	}
}

void Ball::move() {
	Uint32 deltaTime = m_sys->getDeltaTime();
	vect2d_t currPos = m_rect.getPos();
	currPos.x += (long) (m_velocity.x /* * deltaTime */ * 1);
	currPos.y += (long) (m_velocity.y /* * deltaTime */ * 1);
    
	m_rect.setPos(currPos.x, currPos.y);
}

bool Ball::checkCollisionBetweenPos(Grid* pGrid, vect2d_t currBallCenter, vect2d_t nextBallCenter, uint* pCollidingBrickId, vect2d_t* pCollisionPoint, int* pCollisionType) {
	vect2d_t deltaPos;
	deltaPos.x = nextBallCenter.x - currBallCenter.x;
	deltaPos.y = nextBallCenter.y - currBallCenter.y;

	long ballSize = getRect()->getSize().w;
	long ballRadius = ballSize / 2;

	double deltaDist = sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);

	//printf("start check (%d, %d)->(%d, %d)\n", currBallCenter.x, currBallCenter.y, nextBallCenter.x, nextBallCenter.y);

	for (int i = 0; i < (int)deltaDist; i++) {
		pCollisionPoint->x = currBallCenter.x + (long) (((double)deltaPos.x / deltaDist) * (double)i);
		pCollisionPoint->y = currBallCenter.y + (long) (((double)deltaPos.y / deltaDist) * (double)i);

		//printf("checking: %ld, %ld\n", pCollisionPoint->x, pCollisionPoint->y);

		if (pGrid->checkBrickAtPos(*pCollisionPoint, pCollidingBrickId)) {
			*pCollisionType = 0;
			return true;
		}
		else if (pCollisionPoint->x <= 0 || pCollisionPoint->x >= SCREEN_WIDTH - 1) {
			*pCollisionType = 1;
			return true;
		}
		else if (pCollisionPoint->y <= 0 || pCollisionPoint->y >= SCREEN_HEIGHT - 1) {
			*pCollisionType = 2;
			return true;
		}
		else if (pCollisionPoint->x <= 0 || pCollisionPoint->x >= SCREEN_WIDTH - 1
				&& pCollisionPoint->y <= 0 || pCollisionPoint->y >= SCREEN_HEIGHT - 1) {
			*pCollisionType = 3;
			return true;
		}
	}

	*pCollisionType = -1;

	return false;
}
