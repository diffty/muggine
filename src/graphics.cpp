#include "graphics.hpp"


Graphics::Graphics(System* sys) {
	m_sys = sys;
}

void Graphics::Init() {
#ifdef TARGET_3DS
	gfxInitDefault();

#elif TARGET_WIN
	m_sdlScreenSurface = SDL_GetWindowSurface(m_sys->GetWindow());

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
	return (uint8*) m_sdlScreenSurface->pixels;

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

#elif TARGET_WIN
	//SDL_FillRect(m_sdlScreenSurface, NULL, SDL_MapRGB(m_sdlScreenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(m_sys->GetWindow());

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

#elif TARGET_WIN
	SDL_FreeSurface(m_sdlScreenSurface);

#endif
}