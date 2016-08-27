/**
 * @file win_scene.cxx
 * @desc Win scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "../game/common.hxx"
#include "scene.hxx"
#include "win_scene.hxx"

WinScene::~WinScene()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::~WinScene() called";
	fini();
}

WinScene::WinScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::WinScene() called";
	init();
}

void WinScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::init() called";
}

void WinScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::fini() called";
}

bool WinScene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void WinScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	RECT rc = getScreenRect();
	game->getBigFont()->DrawText(NULL, TEXT("You Win"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0x40, 0xCC, 0x40));

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

