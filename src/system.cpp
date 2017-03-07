#include "system.hpp"


System::System() {
	isMainLoopRunning = true;
    
#ifdef TARGET_WIN
	m_window = NULL;
#endif
}

void System::InitWindow() {
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

	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

	#endif
}

bool System::MainLoop() {
#ifdef TARGET_3DS
	return aptMainLoop();

#elif TARGET_WIN
	m_inputSys.ScanInput();

	while (SDL_PollEvent(&m_event) != 0) {
		if (m_event.type == SDL_QUIT) {
			isMainLoopRunning = false;
		}

		else if (m_event.type == SDL_KEYDOWN || m_event.type == SDL_KEYUP) {
			m_inputSys.RegisterKeyEvent(m_event.type, m_event.key.keysym);
		}

		else if (m_event.type == SDL_MOUSEMOTION || m_event.type == SDL_MOUSEBUTTONDOWN || m_event.type == SDL_MOUSEBUTTONUP) {
			vect2d_t mousePos;
			int x, y;
			
			SDL_GetMouseState(&x, &y);
			mousePos.x = x;
			mousePos.y = y;

			m_inputSys.RegisterMouseEvent(m_event.type, mousePos);
		}
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

Input* System::GetInputSys() {
	return &m_inputSys;
}
