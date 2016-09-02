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
	menuIndex = 0;
}

void GameOverScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::init() called";
	IDirect3DDevice9 *device = Game::instance()->getDevice();
	spriteUI = new Sprite();
	spriteUI->init(device, L".\\res\\ui.png", 2, 2);
}

void GameOverScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::fini() called";
	if (spriteUI) {
		delete spriteUI;
		spriteUI = NULL;
	}
}

bool GameOverScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		if (menuIndex == 0)
			Game::instance()->changeScene(Game::SCENE_STAGE);
		else if (menuIndex == 1)
			Game::instance()->changeScene(Game::SCENE_TITLE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		menuIndex--;
		if (menuIndex < 0)
			menuIndex = 1;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		menuIndex++;
		if (menuIndex > 1)
			menuIndex = 0;
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
	rc.bottom -= 256;
	game->getBigFont()->DrawText(NULL, TEXT("Game Over"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0x00, 0x00));

	rc = getScreenRect();
	rc.top += 32;
	game->getNormalFont()->DrawText(NULL, TEXT("Continue"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
	rc.top += 96;
	game->getNormalFont()->DrawText(NULL, TEXT("Restart"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

	// combining projecting and viewing matrices
	D3DXMATRIX matrixProjection;
	D3DXMatrixOrthoLH(&matrixProjection,
		2.0f, 2.0f,    // width & height
		1.0f, 25.0f);  // near & far view-plane
	D3DXMATRIX matrixView;
	D3DXMatrixLookAtLH(&matrixView,
		&D3DXVECTOR3(0.0f, 0.0f, -3.0f),  // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),   // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction
	device->SetTransform(D3DTS_VIEW, &matrixView);
	device->SetTransform(D3DTS_PROJECTION, &matrixProjection);

	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 0.03f, 0.03f, 1.0f);
	D3DXMATRIX matrixTranslate;
	if (menuIndex == 0) {
		D3DXMatrixTranslation(&matrixTranslate, -0.16f * 33.33f, -(32.0f / 768.0f * 2.0f) * 33.33f / 2, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
		spriteUI->draw(device, 1);
	}
	else if (menuIndex == 1) {
		D3DXMatrixTranslation(&matrixTranslate, -0.16f * 33.33f, -(128.0f / 768.0f * 2.0f) * 33.33f / 2, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
		spriteUI->draw(device, 1);
	}

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

