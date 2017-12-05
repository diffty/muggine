#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_SDL
#include <cstdio>
#include <SDL2/SDL.h>
#endif

#ifdef TARGET_WIN
#include <Windows.h>
#endif

#include "input.hpp"
#include "constants.hpp"
#include <time.h>



class System {
public:
	System();
	~System();

	static System* get();

	void consoleInit();
	void initWindow();

	Input* getInputSys();
	double getTime();
	double getDeltaTime();
	int getRandInt(int iMin, int iMax);

#ifdef TARGET_SDL
	SDL_Window* getWindow();
#endif

	bool mainLoop();
	void initLoop();
	void quitLoop();
	void exit();


private:
	static System* m_pInstance;

	bool m_bIsMainLoopRunning;
	Input m_inputSys;

#ifdef TARGET_SDL
	SDL_Window* m_window;
	SDL_Event m_event;
#endif

	double m_prevLoopTime;
	double m_startLoopTime;
	double m_deltaTime;

#ifdef TARGET_WIN
	LARGE_INTEGER m_tickFrequency;
#endif
};


#endif
