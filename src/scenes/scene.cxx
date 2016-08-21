/**
 * @file scene.cxx
 * @desc Game scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::~Scene() called";
}

Scene::Scene()
	: screenRect()
	, fps(0), frameCount(0), timeStart(0)
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::Scene() called";
	Game *game = Game::instance();

	D3DDEVICE_CREATION_PARAMETERS cp;
	game->getDevice()->GetCreationParameters(&cp);
	GetWindowRect(cp.hFocusWindow, &screenRect);

	timeStart = GetTickCount();
}

void Scene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::init() called";
}

void Scene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::fini() called";
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
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
	}

	wchar_t buffer[64];
	wsprintf(buffer, L"%d\n", fps);
	RECT rc = getScreenRect();
	Game::instance()->getSmallFont()->DrawText(NULL, buffer, -1, &rc, DT_RIGHT | DT_BOTTOM, D3DCOLOR_ARGB(0x80, 0xFF, 0xFF, 0xFF));
}

void Scene::handleActivate(HWND hwnd, bool active)
{
	if (active) {
		BOOST_LOG_TRIVIAL(debug) << "window is activate";
		frameCount = 0;
		timeStart = GetTickCount();
	}
	else {
		BOOST_LOG_TRIVIAL(debug) << "window is deactivated";
	}
}

bool Scene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void Scene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

