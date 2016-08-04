#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene()
	: device(), screenRect()
	, smallFont(), normalFont(), bigFont()
	, fps(0), frameCount(0), timeStart(0)
{
	Game *game = Game::instance();
	device = game->getDevice();

	D3DDEVICE_CREATION_PARAMETERS cp;
	getDevice()->GetCreationParameters(&cp);
	GetWindowRect(cp.hFocusWindow, &screenRect);

	smallFont = game->getSmallFont();
	normalFont = game->getNormalFont();
	bigFont = game->getBigFont();

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
}

void Scene::fini()
{
}

void Scene::computeFPS()
{
	frameCount++;
	ULONGLONG timeCurrent = GetTickCount();
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		fps = (int)(frameCount * 1000 / timeUsage);
		timeStart = timeCurrent;
		frameCount = 0;

		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %d\n", fps);
		OutputDebugStringW(buffer);
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
	}

	wchar_t buffer[64];
	wsprintf(buffer, L"%d\n", fps);
	RECT rc = getScreenRect();
	getSmallFont()->DrawText(NULL, buffer, -1, &rc, DT_RIGHT | DT_BOTTOM, D3DCOLOR_ARGB(0x80, 0xFF, 0xFF, 0xFF));
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
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

