#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL2/SDL.h>
#endif

#include "common_types.hpp"
#include "system.hpp"
#include "drawing.hpp"


class Graphics {
public:
	Graphics(System* sys);
	void Init();
	void FillWithColor(uint8 colorHex);
	void SetDoubleBuffering(bool isActive);
	uint8* GetFramebuffer();
	void FlushBuffer();
	void SwapBuffer();
	void WaitForBlank();
	void Exit();

private:
#ifdef TARGET_SDL
	SDL_Surface* m_sdlScreenSurface;
	uint8* m_frameBuffer;
#endif
	
	System* m_sys;
};


#endif
