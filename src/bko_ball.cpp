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

vect2d_t Ball::getCenterPos() {
	vect2d_t centerPos = m_rect.getPos();
	centerPos.x += m_rect.getSize().w;
	centerPos.y += m_rect.getSize().h;
	return centerPos;
}

void Ball::update() {
	if (m_bIsMoving) {
		move();
	}
}

void Ball::move() {
	Uint32 deltaTime = m_sys->getDeltaTime();
	vect2d currPos = m_rect.getPos();
	currPos.x += m_velocity.x * deltaTime;
	currPos.y += m_velocity.y * deltaTime;
	m_rect.setPos(currPos.x, currPos.y);
}