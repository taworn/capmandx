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
	, image()
	, modelX(0.0f), modelY(0.0f), modelDx(0.0f), modelDy(0.0f)
{
	init();
}

void PlayScene::init()
{
	D3DXCreateTextureFromFile(Game::instance()->getDevice(), L"res\\a.png", &image);
}

void PlayScene::fini()
{
	if (image != NULL) {
		image->Release();
		image = NULL;
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
		modelDx = 0.0f;
		modelDy = 0.05f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		modelDx = 0.0f;
		modelDy = -0.05f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		modelDx = -0.05f;
		modelDy = 0.0f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		modelDx = 0.05f;
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
	D3DXMatrixScaling(&matrixScale, 0.1f, 0.1f, 1.0f);
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, modelX, modelY, 0);
	if (modelDx > 0.0f && modelX < 0.75f)
		modelX += modelDx;
	else if (modelDx < 0.0f && modelX > -0.75f)
		modelX += modelDx;
	if (modelDy > 0.0f && modelY < 0.75f)
		modelY += modelDy;
	else if (modelDy < 0.0f && modelY > -0.75f)
		modelY += modelDy;
	device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
	Game::instance()->draw(image);

	D3DXMatrixScaling(&matrixScale, 0.05f, 0.05f, 0.05f);
	for (int i = -9; i <= 9; i++) {
		D3DXMatrixTranslation(&matrixTranslate, 0.1f * i, 0.9f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
		Game::instance()->draw(image);
	}
	for (int i = -9; i <= 9; i++) {
		D3DXMatrixTranslation(&matrixTranslate, 0.1f * i, -0.9f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
		Game::instance()->draw(image);
	}
	for (int i = -9; i <= 9; i++) {
		D3DXMatrixTranslation(&matrixTranslate, 0.9f, 0.1f * i, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
		Game::instance()->draw(image);
	}
	for (int i = -9; i <= 9; i++) {
		D3DXMatrixTranslation(&matrixTranslate, -0.9f, 0.1f * i, 0.0f);
		device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
		Game::instance()->draw(image);
	}

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

