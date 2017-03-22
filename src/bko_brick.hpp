#ifndef BKO_BRICK_HPP
#define BKO_BRICK_HPP

#include "sprite.hpp"


class Brick : public Sprite {
public:
	Brick(uint rscId, RscManager* pRscManager)
		: Sprite(rscId, pRscManager) { };
};

#endif