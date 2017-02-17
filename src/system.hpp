#ifdef TARGET_3DS
#include <3ds.h>
#endif

class System {
public:
	System();
	void ConsoleInit();
	bool MainLoop();
	void Exit();

private:
	bool isMainLoopRunning;
};