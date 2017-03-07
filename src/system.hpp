#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_WIN
#include <windows.h>
#include <cstdio>
#include <SDL.h>
#endif

#include "input.hpp"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240


class System {
public:
	System();
	void ConsoleInit();
	void InitWindow(HINSTANCE hInstance, int nCmdShow);

	Input* GetInputSys();

#ifdef TARGET_WIN
	SDL_Window* GetWindow();
#endif

	bool MainLoop();
	void Exit();

private:
	bool isMainLoopRunning;
	Input m_inputSys;

#ifdef TARGET_WIN
	SDL_Window* m_window;
	SDL_Event m_event;
#endif

};


#endif