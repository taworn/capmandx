#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"
#include "play_scene.hxx"

TitleScene::TitleScene() : Scene()
{
	init();
}

void TitleScene::init()
{
}

void TitleScene::fini()
{
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(new PlayScene());
		return true;
	}
	return false;
}

void TitleScene::render(ULONGLONG timeCurrent)
{
	IDirect3DDevice9 *d3dDev = Game::instance()->getDevice();
	d3dDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	d3dDev->BeginScene();
	d3dDev->SetFVF(CUSTOM_FVF);

	fps(timeCurrent);
	d3dDev->EndScene();
	d3dDev->Present(NULL, NULL, NULL, NULL);
}

