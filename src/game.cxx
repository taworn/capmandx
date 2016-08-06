/**
 * @file game.cxx
 * @desc Game engine module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
#include "scenes/play_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;

	if (bigFont != NULL) {
		bigFont->Release();
		bigFont = NULL;
	}
	if (normalFont != NULL) {
		normalFont->Release();
		normalFont = NULL;
	}
	if (smallFont != NULL) {
		smallFont->Release();
		smallFont = NULL;
	}

	singleton = NULL;
}

Game::Game(IDirect3DDevice9 *dev)
	: d3dDev(dev)
	, smallFont(), normalFont(), bigFont()
	, scene()
{
	assert(singleton == NULL);
	singleton = this;

	D3DXCreateFont(getDevice(), 24, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &smallFont);
	D3DXCreateFont(getDevice(), 32, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &normalFont);
	D3DXCreateFont(getDevice(), 64, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &bigFont);

	scene = new TitleScene();
}

bool Game::deviceReset()
{
	return D3DReset();
}

void Game::changeScene(int sceneId)
{
	delete scene;
	switch (sceneId) {
	default:
	case SCENE_DEFAULT:
		scene = new Scene();
		break;
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	}
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	return scene->handleKey(hwnd, key);
}

void Game::render()
{
	HRESULT hr;
	if (SUCCEEDED(hr = d3dDev->TestCooperativeLevel())) {
		scene->render();
	}
	else if (hr == D3DERR_DEVICELOST) {
		Sleep(1);
	}
	else if (hr == D3DERR_DEVICENOTRESET) {
		if (Game::instance()->deviceReset()) {
			scene->reset();
		}
	}
}

