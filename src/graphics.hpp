#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_WIN
#include <windows.h>
#endif

#include "common_types.hpp"


class Graphics {
public:
	Graphics();

	void Init();
	void SetDoubleBuffering(bool isActive);
	fb_t* GetFramebuffer();
	void DrawFramebuffer(HWND hwnd);
	void FlushBuffer();
	void SwapBuffer();
	void WaitForBlank();
	void Exit();

private:
	//System* m_sys;
	fb_t* m_pFramebuffer;
};


#endif