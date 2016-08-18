/**
 * @file game.cxx
 * @desc Game engine module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
#include "scenes/play_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;
	fini();
	singleton = NULL;
}

Game::Game(IDirect3DDevice9 *dev)
	: d3dDev(dev)
	, smallFont(), normalFont(), bigFont()
	, textureVerticesBuffer()
	, scene()
{
	assert(singleton == NULL);
	singleton = this;
	init();
	scene = new TitleScene();
}

void Game::changeScene(int sceneId)
{
	BOOST_LOG_TRIVIAL(debug) << "changeScene() called, sceneId = " << sceneId;

	delete scene;

	switch (sceneId) {
	default:
	case SCENE_DEFAULT:
		scene = new Scene();
		break;
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	}
}

void Game::handleActivate(HWND hwnd, bool active)
{
	scene->handleActivate(hwnd, active);
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	return scene->handleKey(hwnd, key);
}

void Game::render()
{
	HRESULT hr;
	if (SUCCEEDED(hr = d3dDev->TestCooperativeLevel())) {
		scene->render();
	}
	else if (hr == D3DERR_DEVICELOST) {
		Sleep(1);
	}
	else if (hr == D3DERR_DEVICENOTRESET) {
		scene->fini();
		fini();
		if (D3DReset()) {
			init();
			scene->init();
		}
	}
}

void Game::draw(IDirect3DTexture9 *image)
{
	IDirect3DDevice9 *device = getDevice();
	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	device->SetStreamSource(0, textureVerticesBuffer, 0, sizeof(TEXTURE_VERTEX));
	device->SetTexture(0, image);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Game::init()
{
	D3DXCreateFont(getDevice(), 24, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &smallFont);
	D3DXCreateFont(getDevice(), 32, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &normalFont);
	D3DXCreateFont(getDevice(), 64, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &bigFont);

	TEXTURE_VERTEX verticesData[] = {
		// vertex             // coord
		{ -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, },
		{ 1.0f, 1.0f, 0.0f,   1.0f, 0.0f, },
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, },
		{ 1.0f, -1.0f, 0.0f,  1.0f, 1.0f, },
	};
	getDevice()->CreateVertexBuffer(sizeof(verticesData), D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &textureVerticesBuffer, NULL);
	void *verticesMemory;
	textureVerticesBuffer->Lock(0, sizeof(verticesData), &verticesMemory, NULL);
	memcpy(verticesMemory, verticesData, sizeof(verticesData));
	textureVerticesBuffer->Unlock();
}

void Game::fini()
{
	if (textureVerticesBuffer != NULL) {
		textureVerticesBuffer->Release();
		textureVerticesBuffer = NULL;
	}

	if (bigFont != NULL) {
		bigFont->Release();
		bigFont = NULL;
	}
	if (normalFont != NULL) {
		normalFont->Release();
		normalFont = NULL;
	}
	if (smallFont != NULL) {
		smallFont->Release();
		smallFont = NULL;
	}
}

