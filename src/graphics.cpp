#include "graphics.hpp"
#include <stdio.h>

Graphics::Graphics() {

}

/*Graphics::Graphics(System* sys) {
	m_sys = sys;
}*/

void Graphics::Init() {
#ifdef TARGET_3DS
	gfxInitDefault();
#elif TARGET_WIN
	m_pFramebuffer = (fb_t*) malloc(1440*842*3);
#endif
}

void Graphics::SetDoubleBuffering(bool isActive) {
#ifdef TARGET_3DS
	gfxSetDoubleBuffering(GFX_BOTTOM, isActive);
#endif
}

fb_t* Graphics::GetFramebuffer() {
#ifdef TARGET_3DS
	return gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
#elif TARGET_WIN
	printf("Framebuffer adress : %p\n", m_pFramebuffer);
	return m_pFramebuffer;
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

void Graphics::DrawFramebuffer(HWND hwnd) {
#ifdef TARGET_WIN
	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(hwnd, &ps);

	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

	RECT rect;
	GetWindowRect(hwnd, &rect);

	RGBQUAD m_rgbPalette[256];
	for (int i = 0; i < 255; i++) {
		m_rgbPalette[i].rgbBlue = i;
		m_rgbPalette[i].rgbGreen = i;
		m_rgbPalette[i].rgbRed = i;
	}
	
	LPBITMAPINFO lpbi;
	lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))];

	lpbi->bmiHeader.biWidth = rect.right - rect.left;
	lpbi->bmiHeader.biHeight = rect.bottom - rect.top;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 24;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = (lpbi->bmiHeader.biWidth * 8) * lpbi->bmiHeader.biHeight;
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	memcpy(lpbi->bmiColors, m_rgbPalette, sizeof(m_rgbPalette));

	printf("%dx%d\n", lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight);

	HDC hdcMem = CreateCompatibleDC(hdc);

	fb_t* pbData = GetFramebuffer();

	for (int i = 0; i < 2000; i++)
		pbData[i] = i%255;

	HBITMAP hbm = CreateDIBSection(hdcMem, lpbi, DIB_RGB_COLORS, (void**) &pbData, NULL, 0);

	HBITMAP oldBm = (HBITMAP) SelectObject(hdcMem, hbm);

	BitBlt(hdc, 0, 0, lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight, hdcMem, 0, 0, SRCCOPY);

	StretchDIBits(
		hdc,
		0, 0, lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight,
		0, 0, lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight,
		pbData, lpbi,
		DIB_RGB_COLORS, SRCCOPY);

	//DeleteObject(SelectObject(hdcMem, hbm));

	EndPaint(hwnd, &ps);
#endif
}

