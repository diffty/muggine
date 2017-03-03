#include "system.hpp"


System::System() {
	isMainLoopRunning = true;
}

System::System(Graphics* pGfx) {
	isMainLoopRunning = true;
	m_pGfxSys = pGfx;
}

void System::InitWindow(HINSTANCE hInstance, int nCmdShow) {
	#ifdef TARGET_WIN

	const wchar_t CLASS_NAME[] = L"Muggine Test Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = System::s_WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (LPSTR) CLASS_NAME;

	RegisterClass(&wc);

	// Create the window
	HWND hwnd = CreateWindowEx(
		0,
		(LPSTR) CLASS_NAME,
		"JESUISMUGGINE",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,

		NULL,
		NULL,
		hInstance,
		this
		);

	ShowWindow(hwnd, nCmdShow);

	m_hwnd = hwnd;

	#endif
}

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_PAINT:
		{
			m_pGfxSys->DrawFramebuffer(hwnd);
		}
		return 0;

		case WM_CLOSE: 
			DestroyWindow(hwnd);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			isMainLoopRunning = false;
			return 0;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK System::s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	System *pThis;

	if (uMsg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<System*>(lpcs->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		pThis = reinterpret_cast<System*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis) {
		return pThis->WndProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND System::GetWindow() {
	return m_hwnd;
}

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

	MSG msg = {};

	if (GetMessage(&msg, NULL, 0, 0)) { // PeekMessage(&msg, NULL, 0, 0, PM_REMOVE
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// printf("%u\n", msg.message);

	/*if (isMainLoopRunning)
		printf("isMainLoopRunning\n");
	else
		printf("NOT isMainLoopRunning\n");*/

	return isMainLoopRunning;

	#else
	
	return isMainLoopRunning;
	
	#endif
}

void System::Exit() {

}