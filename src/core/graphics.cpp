#include "graphics.hpp"

#include "../graphics/drawing.hpp"


// Hack because there's missing symbols in SDL1.2 derp
#if (TARGET_SDL || TARGET_SDL2) && _MSC_VER > 1700
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
#endif


Graphics::Graphics(System* sys) {
	m_sys = sys;

    #if TARGET_SDL || TARGET_SDL2
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

#if TARGET_SDL
// TODO: mettre autre part
void blitScaled(SDL_Surface* src, SDL_Surface* dst) {
	int srcW = src->w;
	int srcH = src->h;
	int dstW = dst->w;
	int dstH = dst->h;

	int srcBufSize = srcW * srcH;
	int dstBufSize = dstW * dstH;
	int scaleRatio = dstW / srcW;

	int bpp = src->format->BytesPerPixel;

	for (int i = srcBufSize - 1; i >= 0; i--) {
		for (int j = 0; j < scaleRatio; j++)
			memcpy(((Uint32*) dst->pixels + i * scaleRatio + j), ((Uint32*)src->pixels + i), src->format->BytesPerPixel);
	}

	for (int y = srcH - 1; y >= 0; y--) {
		for (int j = 0; j < scaleRatio; j++)
		memcpy(((Uint32*) dst->pixels + dst->w * (y * scaleRatio + j)), ((Uint32*)dst->pixels + dst->w * y), dst->pitch);
	}
}
#endif

void Graphics::SwapBuffer() {
#ifdef TARGET_3DS
	gfxSwapBuffers();

#elif TARGET_SDL
	// SDL_BlitSurface(m_pSDLDrawSurface, &m_drawBufBlitRect, m_pSDLScreenSurface, &m_screenBufBlitRect);
	blitScaled(m_pSDLDrawSurface, m_pSDLScreenSurface);
	SDL_Flip(m_pSDLScreenSurface);

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

#elif TARGET_SDL || TARGET_SDL2
	SDL_FreeSurface(m_pSDLScreenSurface);

#endif
}
