#include "system.hpp"


System::System() {
	isMainLoopRunning = true;
}

void System::ConsoleInit() {
	#ifdef TARGET_3DS
	consoleInit(GFX_TOP, NULL);
	#endif
}

bool System::MainLoop() {
	#ifdef TARGET_3DS
	return aptMainLoop();
	#else
	return isMainLoopRunning;
	#endif
}

void System::Exit() {

}