/**
 * @file winmain.cxx
 * @desc Main program.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "game.hxx"

static Game *game = NULL;

/**
 * Initializes all core modules.
 */
bool Initialize(HWND hwnd)
{
	if (!D3DInit(hwnd))
		return false;
	game = new Game(d3dDev);
	return true;
}

/**
 * Uninitializes all core modules.
 */
void Uninitialize()
{
	if (game) {
		delete game;
		game = NULL;
	}
	D3DUninit();
}

/**
 * The main window function.
 */
LRESULT CALLBACK
WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		if (!game || !game->handleKey(hwnd, wParam)) {
			if (wParam == VK_ESCAPE)
				PostMessage(hwnd, WM_CLOSE, 0, 0);
		}
		return 0;

	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			active = true;
		else
			active = false;
		if (game)
			game->handleActivate(hwnd, active);
		return 0;

	case WM_DESTROY:
		Uninitialize();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 * Main program for Windows.
 */
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BOOST_LOG_TRIVIAL(trace) << "CapmanDX";

#ifdef _DEBUG
#ifdef _MSC_VER
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
#endif

	// registers window class
	const TCHAR windowClassName[] = TEXT("capmandx");
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = 0;
	RegisterClassEx(&wc);

	// creates window
	HWND hwnd = CreateWindowEx(0,
		windowClassName, TEXT("Capman"),
		WS_POPUP,
		0, 0, 1024, 768,
		HWND_DESKTOP, 0, hInstance, NULL);
	if (hwnd == 0) {
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	// initializes all core modules
	if (!Initialize(hwnd)) {
		Uninitialize();
		return 0;
	}

	// main loop
	MSG msg;
	while (1) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
		if (active)
			game->render();
	}

	return (int)msg.wParam;
}

