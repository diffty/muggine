#ifndef BKO_BRICK_HPP
#define BKO_BRICK_HPP

#include "sprite.hpp"
#include "bko_ball.hpp"


class Brick : public Sprite {
public:
	Brick(uint rscId, RscManager* pRscManager)
		: Sprite(rscId, pRscManager) { };

	uint getBallSideFromBrick(Ball* ball);
};

#endif