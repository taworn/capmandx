/**
 * @file title_scene.cxx
 * @desc Title scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"

TitleScene::~TitleScene()
{
	fini();
}

TitleScene::TitleScene() : Scene(), titleFont(), anim(), modelX(0.0f)
{
	init();
}

void TitleScene::init()
{
	IDirect3DDevice9 *device = Game::instance()->getDevice();
	D3DXCreateFont(device, 128, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &titleFont);
	anim = new Animation();
	anim->init(device, L"res\\a.png", 2, 2, 1000);
	anim->setParameters(0, 2, 500);
}

void TitleScene::fini()
{
	if (anim) {
		delete anim;
		anim = NULL;
	}
	if (titleFont != NULL) {
		titleFont->Release();
		titleFont = NULL;
	}
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_PLAY);
		return true;
	}
	return false;
}

void TitleScene::render()
{
	Game *game = Game::instance();
	IDirect3DDevice9 *device = game->getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();

	RECT rc = getScreenRect();
	rc.bottom -= 256;
	titleFont->DrawText(NULL, TEXT("Capman"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0x80));
	rc = getScreenRect();
	rc.top += 512;
	game->getNormalFont()->DrawText(NULL, TEXT("Press ENTER to Start"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

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
	D3DXMatrixScaling(&matrixScale, 0.2f, 0.2f, 1.0f);
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, modelX, -0.2f, 0);
	modelX -= 0.01f;
	if (modelX < -1.5f)
		modelX = 1.5f;
	device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixTranslate));
	anim->draw(device);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

