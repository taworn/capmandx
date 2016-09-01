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

	GameData::instance()->clear();
	map.load(".\\res\\debug.map");
	//map.load(".\\res\\test0.map");
	//map.load(".\\res\\test1.map");
	spriteMap->prepareBatch(Game::instance()->getDevice(), map.getWidth(), map.getHeight());
	for (int i = 0; i < 4; i++) {
		movDivoes[i].setId(i);
		movDivoes[i].setMap(&map);
		GameData::instance()->addDivo(&movDivoes[i]);
	}
	movHero.setMap(&map);
	timeStart = GetTickCount();
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";
	IDirect3DDevice9 *device = Game::instance()->getDevice();
	spriteMap = new Sprite();
	spriteMap->init(device, L".\\res\\map0.png", 2, 2);
	spritePacman = new Sprite();
	spritePacman->init(device, L".\\res\\pacman.png", 8, 8);

	if (map.getWidth() > 0 && map.getHeight() > 0)
		spriteMap->prepareBatch(device, map.getWidth(), map.getHeight());
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
		BOOST_LOG_TRIVIAL(debug) << "SPACE keydown";
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		BOOST_LOG_TRIVIAL(debug) << "A -or- LEFT keydown";
		movHero.move(Movable::MOVE_LEFT);
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		BOOST_LOG_TRIVIAL(debug) << "D -or- RIGHT keydown";
		movHero.move(Movable::MOVE_RIGHT);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		BOOST_LOG_TRIVIAL(debug) << "W -or- UP keydown";
		movHero.move(Movable::MOVE_UP);
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		BOOST_LOG_TRIVIAL(debug) << "S -or- DOWN keydown";
		movHero.move(Movable::MOVE_DOWN);
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

	// sets timing
	ULONGLONG timeUsed = GetTickCount() - timeStart;
	timeStart = GetTickCount();
	//BOOST_LOG_TRIVIAL(trace) << "used " << timeUsed << " ms";
	GameData::instance()->update(timeUsed);
	movDivoes[0].play(timeUsed);
	movDivoes[1].play(timeUsed);
	movDivoes[2].play(timeUsed);
	movDivoes[3].play(timeUsed);
	movHero.play(timeUsed);
	movHero.detect();

	// combining projecting and viewing matrices
	D3DXMATRIX matrixProjection;
	D3DXMatrixOrthoLH(&matrixProjection,
		2.0f, 2.0f,    // width & height
		1.0f, 25.0f);  // near & far view-plane
	D3DXMATRIX matrixView;
	if (map.getWidth() > 16 || map.getHeight() > 16) {
		D3DXMatrixLookAtLH(&matrixView,
			&D3DXVECTOR3(movHero.getCurrentX(), movHero.getCurrentY(), -10.0f),  // the camera position
			&D3DXVECTOR3(movHero.getCurrentX(), movHero.getCurrentY(), 1.0f),   // the look-at position
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction
	}
	else {
		D3DXMatrixLookAtLH(&matrixView,
			&D3DXVECTOR3(0.0f, 0.0f, -10.0f),  // the camera position
			&D3DXVECTOR3(0.0f, 0.0f, 1.0f),   // the look-at position
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction
	}
	device->SetTransform(D3DTS_VIEW, &matrixView);
	device->SetTransform(D3DTS_PROJECTION, &matrixProjection);

	// drawing map
	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 1.0f, 1.0f, 1.0f);
	map.draw(device, spriteMap, &matrixScale);

	// drawing movables
	D3DXMatrixScaling(&matrixScale, 0.0625f, 0.0625f, 1.0f);
	POINTFLOAT scaleUp;
	scaleUp.x = 16.0f;
	scaleUp.y = 16.0f;
	movDivoes[0].draw(device, spritePacman, &matrixScale, &scaleUp);
	movDivoes[1].draw(device, spritePacman, &matrixScale, &scaleUp);
	movDivoes[2].draw(device, spritePacman, &matrixScale, &scaleUp);
	movDivoes[3].draw(device, spritePacman, &matrixScale, &scaleUp);
	movHero.draw(device, spritePacman, &matrixScale, &scaleUp);

	// checks idling
	for (int i = 0; i < 4; i++) {
		if (movDivoes[i].isIdle())
			movDivoes[i].nextAction();
	}

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

