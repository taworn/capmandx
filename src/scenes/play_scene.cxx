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
#include "../game/common.hxx"
#include "scene.hxx"
#include "play_scene.hxx"

PlayScene::~PlayScene()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::~PlayScene() called";
	/*
	for (int i = 0; i < 4; i++) {
		if (aniDivoes[i]) {
			delete aniDivoes[i];
			aniDivoes[i] = NULL;
		}
	}
	*/
	if (movHero) {
		delete movHero;
		movHero = NULL;
	}
	if (map) {
		delete map;
		map = NULL;
	}
	fini();
}

PlayScene::PlayScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::PlayScene() called";
	init();

	map = new Map();
	map->load();

	const int TIME = 300;
	movHero = new Movable();
	movHero->add(0, 0, 2, TIME);
	movHero->add(1, 2, 4, TIME);
	movHero->add(2, 4, 6, TIME);
	movHero->add(3, 6, 8, TIME);
	movHero->setMap(map);
	movHero->use(0);

	/*
	for (int i = 0; i < 4; i++) {
		aniDivoes[i] = new Animation();
		aniDivoes[i]->add(0, (i + 1) * 8 + 0, (i + 1) * 8 + 2, TIME);
		aniDivoes[i]->add(1, (i + 1) * 8 + 2, (i + 1) * 8 + 4, TIME);
		aniDivoes[i]->add(2, (i + 1) * 8 + 4, (i + 1) * 8 + 6, TIME);
		aniDivoes[i]->add(3, (i + 1) * 8 + 6, (i + 1) * 8 + 8, TIME);
		aniDivoes[i]->use(0);
	}
	*/
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";
	IDirect3DDevice9 *device = Game::instance()->getDevice();

	spritePacman = new Sprite();
	spritePacman->init(device, L".\\res\\pacman.png", 8, 8);

	spriteMap = new Sprite();
	spriteMap->init(device, L".\\res\\map64.png", 2, 2);
}

void PlayScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::fini() called";
	if (spriteMap) {
		delete spriteMap;
		spriteMap = NULL;
	}
	if (spritePacman) {
		delete spritePacman;
		spritePacman = NULL;
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
		//movHero->setVelocity(0.0f, 0.01f);
		//movHero->use(2);
		movHero->move(Map::MOVE_UP);
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		//movHero->setVelocity(0.0f, -0.01f);
		//movHero->use(3);
		movHero->move(Map::MOVE_DOWN);
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		//movHero->setVelocity(-0.01f, 0.0f);
		//movHero->use(0);
		movHero->move(Map::MOVE_LEFT);
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		//movHero->setVelocity(0.01f, 0.0f);
		//movHero->use(1);
		movHero->move(Map::MOVE_RIGHT);
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

	map->draw(device, spriteMap);

	// scaling and translating
	D3DXMATRIX matrixScale;
	//D3DXMatrixScaling(&matrixScale, 0.1f, 0.15f, 1.0f);
	D3DXMatrixScaling(&matrixScale, 0.0625f, 0.0625f, 1.0f);
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, movHero->getCurrentX() * 16, movHero->getCurrentY() * 16, 0);
	/*
	bool enableX = false, enableY = false;
	if (movHero->getVelocityX() > 0.0f && movHero->getCurrentX() < 0.95f)
		enableX = true;
	else if (movHero->getVelocityX() < 0.0f && movHero->getCurrentX() > -0.95f)
		enableX = true;
	if (movHero->getVelocityY() > 0.0f && movHero->getCurrentY() < 0.95f)
		enableY = true;
	else if (movHero->getVelocityY() < 0.0f && movHero->getCurrentY() > -0.95f)
		enableY = true;
	movHero->playFrame(enableX, enableY);
	*/
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movHero->draw(device, spritePacman);

	/*
	D3DXMatrixTranslation(&matrixTranslate, -0.5f * 25, 0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[0]->draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, 0.5f * 25, 0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[1]->draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, -0.5f * 25, -0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[2]->draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, 0.5f * 25, -0.5f * 25, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	aniDivoes[3]->draw(device, spritePacman);
	*/

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

