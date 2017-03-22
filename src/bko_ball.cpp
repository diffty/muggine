#include "bko_ball.hpp"


void Ball::setVelocity(vect2d_t newVelocity) {
	m_velocity = newVelocity;
}

void Ball::setVelocity(uint x, uint y) {
	m_velocity.x = x;
	m_velocity.y = y;
}

vect2d_t Ball::getVelocity() {
	return m_velocity;
}

void Ball::update() {
	if (m_bIsMoving) {
		move();
	}
}

void Ball::move() {
	vect2d currPos = m_rect.getPos();
	currPos.x += m_velocity.x;
	currPos.y += m_velocity.y;
	m_rect.setPos(currPos.x, currPos.y);
}