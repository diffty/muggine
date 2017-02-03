#ifndef DRAWING_HPP
#define DRAWING_HPP


#include <3ds.h>
#include <cmath>
#include <cstring>
#include "common_types.hpp"
#include "mathf.hpp"


void drawBox(u8* fb, int fromX, int fromY, int toX, int toY, Color* color);


#endif