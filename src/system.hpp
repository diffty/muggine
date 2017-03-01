#ifdef TARGET_3DS
#include <3ds.h>
#endif

#ifdef TARGET_WIN
#include <windows.h>
#include <cstdio>
#endif


class System {
public:
	System();
	void ConsoleInit();
	void InitWindow(HINSTANCE hInstance, int nCmdShow);
	bool MainLoop();
	void Exit();
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	bool isMainLoopRunning;
};