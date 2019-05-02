#include "system.hpp"


System* System::m_pInstance = NULL;


System::System() {
#if TARGET_WIN
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&m_tickFrequency);
#endif

	m_bIsMainLoopRunning = true;
	initLoop();

#if TARGET_SDL2
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
#if TARGET_SDL

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		m_pWindowSurface = SDL_SetVideoMode(
			SCREEN_WIDTH * SCREEN_SCALE,
			SCREEN_HEIGHT * SCREEN_SCALE,
			32,
			SDL_SWSURFACE
		);

		if (m_pWindowSurface == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
	}

#elif TARGET_SDL2

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		m_window = SDL_CreateWindow("Hyper Drift Mayor",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH * SCREEN_SCALE,
									SCREEN_HEIGHT * SCREEN_SCALE,
									SDL_WINDOW_SHOWN);
		
		if (m_window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}

		m_pWindowSurface = SDL_GetWindowSurface(m_window);
	}

#endif
}

#if TARGET_SDL2
SDL_Window* System::getWindow() {
	return m_window;
}
#endif

#if TARGET_SDL || TARGET_SDL2
SDL_Surface* System::getWindowSurface() {
	return m_pWindowSurface;
}
#endif

void System::initConsole() {
#if TARGET_3DS
	
	consoleInit(GFX_TOP, NULL);
	
#elif TARGET_SDL || TARGET_SDL2

	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

#endif
}

double System::getTime() {
#if TARGET_WIN
	LARGE_INTEGER currTick;
	LARGE_INTEGER t;

	if (QueryPerformanceCounter(&t) != 0)
		currTick.QuadPart = t.QuadPart;

	double test = ((((double) currTick.QuadPart / (double) m_tickFrequency.QuadPart)));
	return test;
	//return (uint32) (currTick.QuadPart / (freq.QuadPart / 1000000));
    
#elif TARGET_OSX
    timespec timeStruct;
    
/*#if MAC_OS_X_VERSION_MIN_REQUIRED < 101200
    clock_gettime(CLOCK_REALTIME, &timeStruct);
#else*/
    clock_serv_t cclock;
    mach_timespec_t machTimeStruct;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &machTimeStruct);
    mach_port_deallocate(mach_task_self(), cclock);
    timeStruct.tv_sec = machTimeStruct.tv_sec;
    timeStruct.tv_nsec = machTimeStruct.tv_nsec;
/*#endif*/
    
    return ((double) timeStruct.tv_nsec / 1000000000) + timeStruct.tv_sec;

#elif TARGET_SDL || TARGET_SDL2
	return SDL_GetTicks();

#elif TARGET_3DS
    return ((double) osGetTime() / 1000.);

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

int System::getRandInt(int iMin, int iMax) {
    srand((uint)((System::get()->getTime() - (long)System::get()->getTime()) * 1000000000));
    return iMin + rand() % (iMax - iMin);
}

float System::getRandFloat(float fMin, float fMax) {
    srand((uint)((System::get()->getTime() - (long)System::get()->getTime()) * 1000000000));
    return fMin + (((float) rand()) / RAND_MAX) * (fMax - fMin);
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

#if TARGET_3DS
	hidScanInput();
	return aptMainLoop();

#elif TARGET_SDL || TARGET_SDL2
	while (SDL_PollEvent(&m_event) != 0) {
		if (m_event.type == SDL_QUIT) {
			m_bIsMainLoopRunning = false;
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

	return m_bIsMainLoopRunning;

#else
	return m_bIsMainLoopRunning;

#endif
}

void System::quitLoop() {
	m_bIsMainLoopRunning = false;
}

void System::exit() {

#if TARGET_SDL

	SDL_Quit();

#elif TARGET_SDL2

	SDL_DestroyWindow(m_window);
	SDL_Quit();

#endif
}

Input* System::getInputSys() {
	return &m_inputSys;
}
