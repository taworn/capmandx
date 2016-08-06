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
	, angle(0.0f), angleToPlus(3.0f)
	, verticesBuffer(NULL)
{
	init();
}

void PlayScene::reset()
{
	fini();
	Scene::fini();
	Scene::init();
	init();
}

void PlayScene::init()
{
	// initializes with a triangle buffer
	getDevice()->CreateVertexBuffer(3 * sizeof(CUSTOM_VERTEX), 0, CUSTOM_FVF, D3DPOOL_MANAGED, &verticesBuffer, NULL);
	assert(verticesBuffer);

	// copies vertices to the buffer
	CUSTOM_VERTEX verticesData[] = {
	    { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
	    { 0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
	    { 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
	};
	void *p;
	verticesBuffer->Lock(0, 0, (void**)&p, 0);
	memcpy(p, verticesData, sizeof(verticesData));
	verticesBuffer->Unlock();
}

void PlayScene::fini()
{
	if (verticesBuffer != NULL) {
		verticesBuffer->Release();
		verticesBuffer = NULL;
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
		modelDy = -0.1f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		modelDy = 0.1f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		modelDx = -0.1f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		modelDx = 0.1f;
		return true;
	}
	return false;
}

void PlayScene::render()
{
	IDirect3DDevice9 *device = getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();
	device->SetFVF(CUSTOM_FVF);

	// translating
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, modelX + modelDx, modelY + modelDy, 0);
	modelX += modelDx;
	modelY += modelDy;
	modelDx = 0.0f;
	modelDy = 0.0f;

	// rotating
	D3DXMATRIX matrixRotateY;
	D3DXMatrixRotationY(&matrixRotateY, D3DXToRadian(angle));
	angle += angleToPlus;
	if (angle > 89.0f || angle < -89.0f)
		angleToPlus = -angleToPlus;

	// viewing
	D3DXMATRIX matrixView;
	D3DXMatrixLookAtLH(&matrixView,
		&D3DXVECTOR3(0.0f, 0.0f, -3.0f),  // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),   // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction

	// projecting
	D3DXMATRIX matrixProjection;
	D3DXMatrixPerspectiveFovLH(&matrixProjection,
		D3DXToRadian(45),  // the horizontal field of view
		1.3333f,           // aspect ratio
		1.0f,              // the near view-plane
		10.0f);            // the far view-plane

	// draws vertex buffer to display
	device->SetTransform(D3DTS_WORLD, &(matrixRotateY * matrixTranslate));
	device->SetTransform(D3DTS_VIEW, &matrixView);
	device->SetTransform(D3DTS_PROJECTION, &matrixProjection);
	device->SetStreamSource(0, verticesBuffer, 0, sizeof(CUSTOM_VERTEX));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

