#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_WIN
#include <windows.h>
#include <cstdio>
#endif

#include "graphics.hpp"


class System {
public:
	System();
	System(Graphics* pGfx);
	void ConsoleInit();
	bool MainLoop();
	void Exit();

#ifdef TARGET_WIN
	void InitWindow(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND GetWindow();
#endif

private:
	bool isMainLoopRunning;

	Graphics* m_pGfxSys;

#ifdef TARGET_WIN
	HWND m_hwnd;
#endif

};

#endif