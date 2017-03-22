#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP


#include "common_types.hpp"
#include "idrawable.hpp"


class IDrawable {
public:
	IDrawable() {};

	virtual void draw(uint8* buffer) = 0;
};


#endif