#include "system.hpp"


System::System() {
	isMainLoopRunning = true;
	m_window = NULL;
}

void System::InitWindow(HINSTANCE hInstance, int nCmdShow) {
	#ifdef TARGET_WIN

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		m_window = SDL_CreateWindow("MUGGINE TEST WINDOW",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH,
									SCREEN_HEIGHT,
									SDL_WINDOW_SHOWN);
		
		if (m_window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
	}

	#endif
}

#ifdef TARGET_WIN
SDL_Window* System::GetWindow() {
	return m_window;
}
#endif

void System::ConsoleInit() {
	#ifdef TARGET_3DS
	
	consoleInit(GFX_TOP, NULL);
	
	#elif TARGET_WIN

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	#endif
}

bool System::MainLoop() {
#ifdef TARGET_3DS
	return aptMainLoop();

#elif TARGET_WIN
	SDL_PollEvent(&m_event);

	if (m_event.type == SDL_QUIT) {
		isMainLoopRunning = false;
	}

	return isMainLoopRunning;

#else
	return isMainLoopRunning;
#endif
}

void System::Exit() {
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}