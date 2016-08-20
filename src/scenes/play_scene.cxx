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
	fini();
}

PlayScene::PlayScene()
	: Scene()
	, spriteMap(), spritePacman()
	, map()
	, movDivoes(), movHero()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::PlayScene() called";
	init();
	map.load();
	for (int i = 0; i < 4; i++) {
		movDivoes[i].setId(i);
		movDivoes[i].setMap(&map);
		movDivoes[i].nextMove();
	}
	movHero.setMap(&map);
	timeStart = GetTickCount();
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";
	IDirect3DDevice9 *device = Game::instance()->getDevice();
	spriteMap = new Sprite();
	spriteMap->init(device, L".\\res\\map64.png", 2, 2);
	spritePacman = new Sprite();
	spritePacman->init(device, L".\\res\\pacman.png", 8, 8);
}

void PlayScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::fini() called";
	if (spritePacman) {
		delete spritePacman;
		spritePacman = NULL;
	}
	if (spriteMap) {
		delete spriteMap;
		spriteMap = NULL;
	}
}

bool PlayScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		BOOST_LOG_TRIVIAL(debug) << L"SPACE keydown";
		return true;
	}
	else if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << L"ENTER keydown";
		Game::instance()->changeScene(SCENE_TITLE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		BOOST_LOG_TRIVIAL(debug) << L"W -or- UP keydown";
		movHero.move(Map::MOVE_UP);
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		BOOST_LOG_TRIVIAL(debug) << L"S -or- DOWN keydown";
		movHero.move(Map::MOVE_DOWN);
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		BOOST_LOG_TRIVIAL(debug) << L"A -or- LEFT keydown";
		movHero.move(Map::MOVE_LEFT);
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		BOOST_LOG_TRIVIAL(debug) << L"D -or- RIGHT keydown";
		movHero.move(Map::MOVE_RIGHT);
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

	ULONGLONG timeUsed = GetTickCount() - timeStart;
	timeStart = GetTickCount();
	//BOOST_LOG_TRIVIAL(trace) << "used: " << timeUsed;

	map.draw(device, spriteMap);

	// scaling and translating
	D3DXMATRIX matrixScale;
	//D3DXMatrixScaling(&matrixScale, 0.1f, 0.15f, 1.0f);
	D3DXMatrixScaling(&matrixScale, 0.0625f, 0.0625f, 1.0f);

	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, movHero.getCurrentX() * 16, movHero.getCurrentY() * 16, 0);
	movHero.play(timeUsed);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movHero.draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, movDivoes[0].getCurrentX() * 16, movDivoes[0].getCurrentY() * 16, 0);
	movDivoes[0].play(timeUsed);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movDivoes[0].draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, movDivoes[1].getCurrentX() * 16, movDivoes[1].getCurrentY() * 16, 0);
	movDivoes[1].play(timeUsed);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movDivoes[1].draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, movDivoes[2].getCurrentX() * 16, movDivoes[2].getCurrentY() * 16, 0);
	movDivoes[2].play(timeUsed);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movDivoes[2].draw(device, spritePacman);

	D3DXMatrixTranslation(&matrixTranslate, movDivoes[3].getCurrentX() * 16, movDivoes[3].getCurrentY() * 16, 0);
	movDivoes[3].play(timeUsed);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
	movDivoes[3].draw(device, spritePacman);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

