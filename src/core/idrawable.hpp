#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP


#include "common_types.hpp"
#include "iobject.hpp"


class IDrawable : public IObject {
public:
	IDrawable() :
		IObject() {};
	virtual ~IDrawable() {};

	virtual void draw(uint8* buffer) = 0;
};


#endif