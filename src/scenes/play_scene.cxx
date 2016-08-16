/**
 * @file play_scene.cxx
 * @desc Playing scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "scene.hxx"
#include "play_scene.hxx"

PlayScene::~PlayScene()
{
	fini();
}

PlayScene::PlayScene()
	: Scene()
	, modelX(0.0f), modelY(0.0f), modelDx(0.0f), modelDy(0.0f)
{
	init();
}

void PlayScene::init()
{
	IDirect3DDevice9 *device = Game::instance()->getDevice();

	sprite = new Sprite();
	sprite->init(device, L".\\res\\pacman.png", 8, 8);

	const int TIME = 300;
	aniHero = new Animation(sprite);
	aniHero->add(0, 0, 2, TIME);
	aniHero->add(1, 2, 4, TIME);
	aniHero->add(2, 4, 6, TIME);
	aniHero->add(3, 6, 8, TIME);
	aniHero->use(0);

	for (int i = 0; i < 4; i++) {
		aniDivoes[i] = new Animation(sprite);
		aniDivoes[i]->add(0, (i + 1) * 8 + 0, (i + 1) * 8 + 2, TIME);
		aniDivoes[i]->add(1, (i + 1) * 8 + 2, (i + 1) * 8 + 4, TIME);
		aniDivoes[i]->add(2, (i + 1) * 8 + 4, (i + 1) * 8 + 6, TIME);
		aniDivoes[i]->add(3, (i + 1) * 8 + 6, (i + 1) * 8 + 8, TIME);
		aniDivoes[i]->use(0);
	}
}

void PlayScene::fini()
{
	for (int i = 0; i < 4; i++) {
		if (aniDivoes[i]) {
			delete aniDivoes[i];
			aniDivoes[i] = NULL;
		}
	}
	if (aniHero) {
		delete aniHero;
		aniHero = NULL;
	}
	if (sprite) {
		delete sprite;
		sprite = NULL;
	}
}

bool PlayScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		OutputDebugStringW(L"SPACE keydown\n");
		return true;
	}
	else if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_TITLE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		OutputDebugStringW(L"W -or- UP keydown\n");
		aniHero->use(2);
		modelDx = 0.0f;
		modelDy = 0.01f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		aniHero->use(3);
		modelDx = 0.0f;
		modelDy = -0.01f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		aniHero->use(0);
		modelDx = -0.01f;
		modelDy = 0.0f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		aniHero->use(1);
		modelDx = 0.01f;
		modelDy = 0.0f;
		return true;
	}
	return false;
}

void PlayScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

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

	// scaling and translating
	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 0.04f, 0.04f, 1.0f);
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, modelX * 25, modelY * 25, 0);
	if (modelDx > 0.0f && modelX < 0.95f)
		modelX += modelDx;
	else if (modelDx < 0.0f && modelX > -0.95f)
		modelX += modelDx;
	if (modelDy > 0.0f && modelY < 0.95f)
		modelY += modelDy;
	else if (modelDy < 0.0f && modelY > -0.95f)
		modelY += modelDy;
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniHero->draw(device);

	D3DXMatrixTranslation(&matrixTranslate, -0.5f * 25, 0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[0]->draw(device);

	D3DXMatrixTranslation(&matrixTranslate, 0.5f * 25, 0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[1]->draw(device);

	D3DXMatrixTranslation(&matrixTranslate, -0.5f * 25, -0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[2]->draw(device);

	D3DXMatrixTranslation(&matrixTranslate, 0.5f * 25, -0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[3]->draw(device);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

