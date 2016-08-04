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
	D3DXCreateFont(getDevice(), 128, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"), &titleFont);
}

void TitleScene::fini()
{
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
	IDirect3DDevice9 *device = getDevice();
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	device->BeginScene();
	device->SetFVF(CUSTOM_FVF);

	RECT rc = getScreenRect();
	rc.bottom -= 256;
	titleFont->DrawText(NULL, TEXT("Capman"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0x80));
	rc = getScreenRect();
	rc.top += 512;
	getNormalFont()->DrawText(NULL, TEXT("Press ENTER to Start"), -1, &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

	computeFPS();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

