/**
 * @file stage_scene.cxx
 * @desc Stage scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "../game/common.hxx"
#include "scene.hxx"
#include "stage_scene.hxx"

StageScene::~StageScene()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::~StageScene() called";
	fini();
}

StageScene::StageScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::StageScene() called";
	init();
	timeStart = GetTickCount();
	timeUsed = 0;
}

void StageScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::init() called";
}

void StageScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::fini() called";
}

void StageScene::handleActivate(HWND hwnd, bool active)
{
	if (active) {
		BOOST_LOG_TRIVIAL(debug) << "window is activate";
		timeStart = GetTickCount();
	}
}

bool StageScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		Game::instance()->changeScene(Game::SCENE_PLAY);
		return true;
	}
	return false;
}

void StageScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	RECT rc = getScreenRect();
	wchar_t buffer[64];
	wsprintf(buffer, L"Stage %d", 1);
	game->getBigFont()->DrawText(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

	ULONGLONG timeUsed = GetTickCount() - timeStart;
	timeStart = GetTickCount();
	this->timeUsed += timeUsed;
	if (this->timeUsed >= 2000)
		Game::instance()->changeScene(Game::SCENE_PLAY);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

