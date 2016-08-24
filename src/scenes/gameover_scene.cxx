/**
 * @file gameover_scene.cxx
 * @desc Game over scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "../game/common.hxx"
#include "scene.hxx"
#include "gameover_scene.hxx"
#include "title_scene.hxx"

GameOverScene::~GameOverScene()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::~GameOverScene() called";
	fini();
}

GameOverScene::GameOverScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::GameOverScene() called";
	init();
}

void GameOverScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::init() called";
}

void GameOverScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::fini() called";
}

bool GameOverScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		Game::instance()->changeScene(Game::SCENE_TITLE);
		return true;
	}
	return false;
}

void GameOverScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	RECT rc = getScreenRect();
	game->getBigFont()->DrawText(NULL, TEXT("Game Over"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0x00, 0x00));

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

