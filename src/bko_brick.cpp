#include "bko_brick.hpp"


/*
uint Brick::getBallSideFromBrick(Ball* ball) {
	vect2d_t brickPos = m_rect.getPos();
	size2d_t brickSize = m_rect.getSize();
	vect2d_t ballPos = ball->getRect()->getPos();
	size2d_t ballSize = ball->getRect()->getSize();
	vect2d_t ballCenter;
	ballCenter.x = ballPos.x + ballSize.w;
	ballCenter.y = ballPos.y + ballSize.h;

	vect2d_t ballVel = ball->getVelocity();
	if (ballCenter.x < brickPos.x || ballCenter.x > brickPos.x + brickSize.w) {
		return 0;
	}

	if (ballCenter.y < brickPos.y || ballCenter.y > brickPos.y + brickSize.h) {
		return 2;
	}

	return 0;
}
*/