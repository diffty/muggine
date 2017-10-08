#include "system.hpp"


System* System::m_pInstance = NULL;


System::System() {
#ifdef TARGET_WIN
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&m_tickFrequency);
#endif

	isMainLoopRunning = true;
	initLoop();

#ifdef TARGET_SDL
	m_window = NULL;
#endif
}

System::~System() {

}

System* System::get() {
	if (!m_pInstance) {
		m_pInstance = new System();
	}

	return m_pInstance;
}


void System::initWindow() {
#ifdef TARGET_SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		m_window = SDL_CreateWindow("MUGGINE TEST WINDOW",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH * SCREEN_SCALE,
									SCREEN_HEIGHT * SCREEN_SCALE,
									SDL_WINDOW_SHOWN);
		
		if (m_window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
	}

	#endif
}

#ifdef TARGET_SDL
SDL_Window* System::getWindow() {
	return m_window;
}
#endif

void System::consoleInit() {
#ifdef TARGET_3DS
	
	consoleInit(GFX_TOP, NULL);
	
#elif TARGET_SDL

	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

#endif
}

double System::getTime() {
#ifdef TARGET_WIN
	LARGE_INTEGER currTick;
	LARGE_INTEGER t;

	if (QueryPerformanceCounter(&t) != 0)
		currTick.QuadPart = t.QuadPart;

	double test = ((((double) currTick.QuadPart / (double) m_tickFrequency.QuadPart)));
	return test;
	//return (uint32) (currTick.QuadPart / (freq.QuadPart / 1000000));
    
#elif TARGET_OSX
    timespec timeStruct;
    clock_gettime(CLOCK_REALTIME, &timeStruct);
    return ((double) timeStruct.tv_nsec / 1000000000) + timeStruct.tv_sec;
    

#elif TARGET_SDL
	return SDL_GetTicks();

#elif TARGET_3DS
    return (uint32) osGetTime();

#elif TARGET_DOS
	double currentTime;
	struct time t_struc;
	gettime(&t_struc);
	currentTime = ((double)time(NULL) + ((double)t_struc.ti_hund)*0.01);
	return currentTime;

#endif
}

double System::getDeltaTime() {
	return m_deltaTime;
}

void System::initLoop() {
	m_startLoopTime = getTime();
	m_prevLoopTime = m_startLoopTime;
	m_deltaTime = m_startLoopTime - m_prevLoopTime;
}

bool System::mainLoop() {
	m_startLoopTime = getTime();
	m_deltaTime = m_startLoopTime - m_prevLoopTime;
	m_prevLoopTime = m_startLoopTime;

#ifdef TARGET_3DS
	hidScanInput();
	return aptMainLoop();

#elif TARGET_SDL
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

			m_inputSys.RegisterMouseEvent(m_event.type, mousePos, m_event.button.button);
		}
	}

	return isMainLoopRunning;

#else
	return isMainLoopRunning;

#endif
}

void System::exit() {
#ifdef TARGET_SDL
	SDL_DestroyWindow(m_window);
	SDL_Quit();
#endif
}

Input* System::getInputSys() {
	return &m_inputSys;
}
