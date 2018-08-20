#include "graphics.hpp"


Graphics::Graphics(System* sys) {
	m_sys = sys;

    #ifdef TARGET_SDL2
	m_drawBufBlitRect.x = 0;
	m_drawBufBlitRect.y = 0;
	m_drawBufBlitRect.w = SCREEN_WIDTH;
	m_drawBufBlitRect.h = SCREEN_HEIGHT;

	m_screenBufBlitRect.x = 0;
	m_screenBufBlitRect.y = 0;
	m_screenBufBlitRect.w = SCREEN_WIDTH * SCREEN_SCALE;
	m_screenBufBlitRect.h = SCREEN_HEIGHT * SCREEN_SCALE;
    #endif
}

void Graphics::Init() {
#ifdef TARGET_3DS
	gfxInitDefault();

#elif TARGET_SDL || TARGET_SDL2
	m_pSDLDrawSurface = SDL_CreateRGBSurface(
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		32,
		0,0,0,0);

	m_pSDLScreenSurface = m_sys->getWindowSurface();

#endif
}

void Graphics::FillWithColor(uint8 colorHex) {
	uint8* fb = GetFramebuffer();

	memset(fb,
		colorHex,
		SCREEN_BPP * SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint8)
		);
}

void Graphics::SetDoubleBuffering(bool isActive) {
#ifdef TARGET_3DS
	gfxSetDoubleBuffering(GFX_BOTTOM, isActive);

#endif
}

uint8* Graphics::GetFramebuffer() {
#ifdef TARGET_3DS
	return gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

#else
	return (uint8*) m_pSDLDrawSurface->pixels;

#endif
}

void Graphics::FlushBuffer() {
#ifdef TARGET_3DS
	gfxFlushBuffers();

#endif
}

void Graphics::SwapBuffer() {
#ifdef TARGET_3DS
	gfxSwapBuffers();

#elif TARGET_SDL
	SDL_BlitSurface(m_pSDLDrawSurface, &m_drawBufBlitRect, m_pSDLScreenSurface, &m_screenBufBlitRect);

#elif TARGET_SDL2
	SDL_BlitScaled(m_pSDLDrawSurface, &m_drawBufBlitRect, m_pSDLScreenSurface, &m_screenBufBlitRect);
	SDL_UpdateWindowSurface(m_sys->getWindow());

#endif
}

void Graphics::WaitForBlank() {
#ifdef TARGET_3DS
	gspWaitForVBlank();

#endif
}

void Graphics::Exit() {
#ifdef TARGET_3DS
	gfxExit();

#elif TARGET_SDL2
	SDL_FreeSurface(m_pSDLScreenSurface);

#endif
}
