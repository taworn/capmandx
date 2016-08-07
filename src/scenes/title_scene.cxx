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

TitleScene::TitleScene() : Scene(), titleFont()
{
	init();
}

void TitleScene::reset()
{
	fini();
	Scene::fini();
	Scene::init();
	init();
}

void TitleScene::init()
{
	IDirect3DDevice9 *device = Game::instance()->getDevice();
	D3DXCreateFont(device, 128, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &titleFont);
	D3DXCreateTextureFromFile(device, L"res\\a.png", &image);

	TEXTURE_VERTEX vertices[] = {
		// Front Face (1-2-3-4)
		{ -1.0f, 1.0f, -1.0f, 0xffffffff, 0.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 0xffffffff, 1.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, 0xffffffff, 0.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff, 1.0f, 1.0f },

		// Right Face (2-6-4-8)
		{ 1.0f, 1.0f, -1.0f, 0xff7f7fff, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0xff7f7fff, 1.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 0xff7f7fff, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 0xff7f7fff, 1.0f, 1.0f },

		// Top Face (5-6-1-2)
		{ -1.0f, 1.0f, 1.0f, 0xff7fff7f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0xff7fff7f, 1.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, 0xff7fff7f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 0xff7fff7f, 1.0f, 1.0f },

		// Back Face (6-5-8-7)
		{ 1.0f, 1.0f, 1.0f, 0xffff7f7f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 0xffff7f7f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 0xffff7f7f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f, 0xffff7f7f, 1.0f, 1.0f },

		// Left Face (5-1-7-3)
		{ -1.0f, 1.0f, 1.0f, 0xffffff7f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, 0xffffff7f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 0xffffff7f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f, 0xffffff7f, 1.0f, 1.0f },

		// Bottom Face (3-4-7-8)
		{ -1.0f, -1.0f, -1.0f, 0xff3f9f3f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 0xff3f3f3f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 0xff3f3f3f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 0xff3f3f3f, 1.0f, 1.0f }
	};

	vertexBuffer = NULL;
	device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, TEXTURE_FVF, D3DPOOL_DEFAULT, &vertexBuffer, NULL);
	void *bufferMemory;
	vertexBuffer->Lock(0, sizeof(vertices), &bufferMemory, NULL);
	memcpy(bufferMemory, vertices, sizeof(vertices));
	vertexBuffer->Unlock();
}

void TitleScene::fini()
{
	if (vertexBuffer != NULL) {
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}
	if (image != NULL) {
		image->Release();
		image = NULL;
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
	device->SetFVF(CUSTOM_FVF);

	RECT rc = getScreenRect();
	rc.bottom -= 256;
	titleFont->DrawText(NULL, TEXT("Capman"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0x80));
	rc = getScreenRect();
	rc.top += 512;
	game->getNormalFont()->DrawText(NULL, TEXT("Press ENTER to Start"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));


	device->SetFVF(TEXTURE_FVF);
	device->SetStreamSource(0, vertexBuffer, 0, 24);
	device->SetTexture(0, image);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// translating
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, 0, -0.4f, -0.4f);

	// rotating
	D3DXMATRIX matrixRotateX, matrixRotateY;
	D3DXMatrixRotationX(&matrixRotateX, D3DXToRadian(45));
	D3DXMatrixRotationY(&matrixRotateY, D3DXToRadian(45));

	// scaling
	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 0.5f, 0.5f, 0.5f);

	// viewing
	D3DXMATRIX matrixView;
	D3DXMatrixLookAtLH(&matrixView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),  // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 5.0f),   // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction

	// projecting
	D3DXMATRIX matrixProjection;
	D3DXMatrixPerspectiveFovLH(&matrixProjection,
		D3DXToRadian(45),  // the horizontal field of view
		1.3333f,           // aspect ratio
		1.0f,              // the near view-plane
		10.0f);            // the far view-plane

	// draws vertex buffer to display
	device->SetTransform(D3DTS_WORLD, &(matrixScale * matrixRotateY * matrixRotateX * matrixTranslate));
	device->SetTransform(D3DTS_VIEW, &matrixView);
	device->SetTransform(D3DTS_PROJECTION, &matrixProjection);
	for (int i = 0; i < 6; i++)
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);


	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

