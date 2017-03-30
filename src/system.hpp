#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_SDL
//#include <windows.h>
#include <cstdio>
#include <SDL2/SDL.h>
#endif

#include "input.hpp"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#ifdef TARGET_3DS
#define SCREEN_BPP		3
#elif TARGET_SDL
#define SCREEN_BPP		4
#endif


class System {
public:
	System();
	void ConsoleInit();
	void InitWindow();

	Input* GetInputSys();
	uint32 getTime();
	uint32 getDeltaTime();

#ifdef TARGET_SDL
	SDL_Window* GetWindow();
#endif

	bool MainLoop();
	void initLoop();
	void Exit();

private:
	bool isMainLoopRunning;
	Input m_inputSys;

#ifdef TARGET_SDL
	SDL_Window* m_window;
	SDL_Event m_event;
#endif

	uint32 m_prevLoopTime;
	uint32 m_startLoopTime;
	uint32 m_deltaTime;
};


#endif
