/**
 * @file next_stage_scene.cxx
 * @desc Next stage scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "../game/common.hxx"
#include "scene.hxx"
#include "next_stage_scene.hxx"

NextStageScene::~NextStageScene()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::~NextStageScene() called";
	fini();
}

NextStageScene::NextStageScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::NextStageScene() called";
	init();
}

void NextStageScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::init() called";
}

void NextStageScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::fini() called";
}

void NextStageScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	if (GameData::instance()->nextStage())
		Game::instance()->changeScene(Game::SCENE_STAGE);
	else
		Game::instance()->changeScene(Game::SCENE_WIN);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

