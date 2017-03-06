#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_WIN
#include <SDL.h>
#endif

#include "common_types.hpp"
#include "system.hpp"


class Graphics {
public:
	Graphics(System* sys);
	void Init();
	void SetDoubleBuffering(bool isActive);
	uint8* GetFramebuffer();
	void FlushBuffer();
	void SwapBuffer();
	void WaitForBlank();
	void Exit();

private:
#ifdef TARGET_WIN
	SDL_Surface* m_sdlScreenSurface;
	uint8* m_frameBuffer;
#endif
	
	System* m_sys;
};


#endif