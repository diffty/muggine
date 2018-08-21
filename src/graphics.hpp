#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL/SDL.h>
#elif TARGET_SDL2
#include <SDL2/SDL.h>
#endif

#include "common_types.hpp"
#include "system.hpp"
#include "drawing.hpp"
#include "constants.hpp"


class Graphics {
private:
#if TARGET_SDL || TARGET_SDL2
	uint8* m_frameBuffer;
	SDL_Surface* m_pSDLScreenSurface;
	SDL_Surface* m_pSDLDrawSurface;
	SDL_Rect m_drawBufBlitRect;
	SDL_Rect m_screenBufBlitRect;

#endif
	System* m_sys;

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
};


#endif
