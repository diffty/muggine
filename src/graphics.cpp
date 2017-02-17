#include "graphics.hpp"


Graphics::Graphics() {

}

void Graphics::Init() {
#ifdef TARGET_3DS
	gfxInitDefault();
#endif
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
	return nullptr;
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
	#endif
}