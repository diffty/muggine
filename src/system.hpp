#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_SDL2
#include <cstdio>
#include <SDL2/SDL.h>
#endif

#ifdef TARGET_WIN
#include <Windows.h>
#endif

#include "input.hpp"
#include "constants.hpp"
#include <time.h>
#include <cstdlib>

#ifdef TARGET_OSX
#include <mach/clock.h>
#include <mach/mach.h>
#endif



class System {
private:
	static System* m_pInstance;

	bool m_bIsMainLoopRunning;
	Input m_inputSys;

#if TARGET_SDL2
	SDL_Window* m_window;
#endif

#if TARGET_SDL || TARGET_SDL2
	SDL_Event m_event;
	SDL_Surface* m_pWindowSurface;
#endif

	double m_prevLoopTime;
	double m_startLoopTime;
	double m_deltaTime;

#if TARGET_WIN
	LARGE_INTEGER m_tickFrequency;
#endif

public:
	System();
	~System();

	static System* get();

	void initConsole();
	void initWindow();

	Input* getInputSys();
	double getTime();
	double getDeltaTime();
    int getRandInt(int iMin, int iMax);
    float getRandFloat(float fMin, float fMax);

#if TARGET_SDL2
	SDL_Window* getWindow();
#endif

#if TARGET_SDL || TARGET_SDL2
	SDL_Surface* getWindowSurface();
#endif

	bool mainLoop();
	void initLoop();
	void quitLoop();
	void exit();
};


#endif
