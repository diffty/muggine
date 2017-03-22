#ifndef BKO_PADDLE_HPP
#define BKO_PADDLE_HPP

#include "sprite.hpp"


class Paddle : public Sprite {
public:
	Paddle(uint rscId, RscManager* pRscManager)
		: Sprite(rscId, pRscManager) { };
};

#endif