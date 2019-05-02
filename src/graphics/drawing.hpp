#ifndef DRAWING_HPP
#define DRAWING_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include <cmath>
#include <cstring>
#include "../core/common_types.hpp"


void drawLine(uint8* buffer, int fromX, int fromY, int toX, int toY, Color* color);
void drawBox(uint8* fb, int fromX, int fromY, int toX, int toY, Color* color);
void rescaleBuffer(uint8* srcBuf, int srcW, int srcH, int srcBpp, uint8* dstBuf, int dstW, int dstH, int dstBpp);

#endif
