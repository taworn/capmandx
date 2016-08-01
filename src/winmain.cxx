#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "game.hxx"

static Game *game = NULL;

bool Initialize(HWND hwnd)
{
	if (!D3DInit(hwnd))
		return false;
	game = new Game(d3dDev);
	return true;
}

void Uninitialize()
{
	if (game) {
		delete game;
		game = NULL;
	}
	D3DUninit();
}

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
		return 0;

	case WM_DESTROY:
		Uninitialize();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BOOST_LOG_TRIVIAL(trace) << "CapmanDX";

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

	HWND hwnd = CreateWindowEx(0,
		windowClassName, TEXT("Capman"),
		WS_POPUP,
		0, 0, 1024, 768,
		HWND_DESKTOP, 0, hInstance, NULL);
	if (hwnd == 0) {
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	if (!Initialize(hwnd)) {
		Uninitialize();
		return 0;
	}

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

