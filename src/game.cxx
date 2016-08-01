#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
#include "scenes/play_scene.hxx"
#include "game.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;
	fini();
	singleton = NULL;
}

Game::Game(IDirect3DDevice9 *dev) : d3dDev(dev)
{
	assert(singleton == NULL);
	singleton = this;
	init();
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
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	case SCENE_DEFAULT:
	default:
		scene = new Scene();
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

void Game::init()
{
}

void Game::fini()
{
}

