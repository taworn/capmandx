#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "scene.hxx"
#include "../game.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene() : device(), screenRect(), frameCount(0), fps(0), timeStart(0), fpsFont()
{
	Game *game = Game::instance();
	device = game->getDevice();

	D3DDEVICE_CREATION_PARAMETERS cp;
	device->GetCreationParameters(&cp);
	GetWindowRect(cp.hFocusWindow, &screenRect);

	timeStart = GetTickCount();

	init();
}

void Scene::reset()
{
	fini();
	init();
}

void Scene::init()
{
	D3DXCreateFont(getDevice(), 24, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &fpsFont);
}

void Scene::fini()
{
	if (fpsFont != NULL) {
		fpsFont->Release();
		fpsFont = NULL;
	}
}

void Scene::computeFPS()
{
	frameCount++;
	ULONGLONG timeCurrent = GetTickCount();
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		fps = frameCount * 1000 / timeUsage;
		timeStart = timeCurrent;
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

void Scene::drawFPS()
{
	RECT rc = getScreenRect();
	wchar_t buffer[64];
	wsprintf(buffer, L"%ld", getFPS());
	fpsFont->DrawText(NULL, buffer, -1, &rc, DT_RIGHT | DT_BOTTOM, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
}

bool Scene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void Scene::render()
{
	IDirect3DDevice9 *device = getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();
	device->SetFVF(CUSTOM_FVF);

	computeFPS();
	drawFPS();

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

