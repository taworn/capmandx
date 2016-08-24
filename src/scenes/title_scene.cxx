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
#include "../game/common.hxx"
#include "scene.hxx"
#include "title_scene.hxx"

TitleScene::~TitleScene()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::~TitleScene() called";
	if (aniHero) {
		delete aniHero;
		aniHero = NULL;
	}
	if (aniDivo) {
		delete aniDivo;
		aniDivo = NULL;
	}
	fini();
}

TitleScene::TitleScene()
	: Scene()
	, titleFont()
	, spritePacman()
	, aniDivo()
	, aniHero()
	, modelX(0.0f)
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::TitleScene() called";
	init();

	const int TIME = 300;
	aniDivo = new Animation();
	aniDivo->add(0, 8, 10, TIME);
	aniDivo->add(1, 10, 12, TIME);
	aniDivo->add(2, 12, 14, TIME);
	aniDivo->add(3, 14, 16, TIME);
	aniDivo->use(0);

	aniHero = new Animation();
	aniHero->add(0, 0, 2, TIME);
	aniHero->add(1, 2, 4, TIME);
	aniHero->add(2, 4, 6, TIME);
	aniHero->add(3, 6, 8, TIME);
	aniHero->use(0);
}

void TitleScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::init() called";
	IDirect3DDevice9 *device = Game::instance()->getDevice();

	D3DXCreateFont(device, 128, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &titleFont);

	spritePacman = new Sprite();
	spritePacman->init(device, L".\\res\\pacman.png", 8, 8);
}

void TitleScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::fini() called";
	if (spritePacman) {
		delete spritePacman;
		spritePacman = NULL;
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
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		Game::instance()->changeScene(Game::SCENE_PLAY);
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

	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 0.05f, 0.05f, 1.0f);
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, modelX * 20, -0.2f * 20, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniHero->draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, (modelX - 0.25f) * 20, -0.2f * 20, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivo->draw(device, spritePacman);

	modelX -= 0.01f;
	if (modelX < -1.0f)
		modelX = 1.0f;

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

