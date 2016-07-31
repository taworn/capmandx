#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;
	singleton = NULL;
}

Game::Game(IDirect3DDevice9 *dev) : d3dDev(dev)
{
	assert(singleton == NULL);
	singleton = this;
	scene = new TitleScene();
}

bool Game::deviceReset()
{
	return D3DReset();
}

void Game::changeScene(Scene *newScene)
{
	delete scene;
	scene = newScene;
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	return scene->handleKey(hwnd, key);
}

void Game::render()
{
	HRESULT hr;
	if (SUCCEEDED(hr = d3dDev->TestCooperativeLevel())) {
		scene->render(GetTickCount());
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

