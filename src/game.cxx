#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
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

void Game::changeScene(Scene *newScene)
{
	delete scene;
	scene = newScene;
}

