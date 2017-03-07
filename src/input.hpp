#ifndef INPUT_HPP
#define INPUT_HPP


#ifdef TARGET_3DS
#include <3ds.h>

#elif TARGET_WIN
#include <SDL2/SDL.h>

#else
#define _CRT_SECURE_NO_WARNINGS

#endif


#include <stdio.h>
#include "common_types.hpp"


class Input {
public:
	Input();
	void ScanInput();
	uint32 GetInput();
	bool IsPressed(uint32 key);
	bool GetTouch(vect2d_t* touch);
	void RegisterKeyEvent(Uint32 eventType, SDL_Keysym key);
	void RegisterMouseEvent(Uint32 eventType, vect2d_t mousePos);

private:

};

#endif
