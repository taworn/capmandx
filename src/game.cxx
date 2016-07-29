#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "game.hxx"

void Game::init()
{
	timeStart = GetTickCount();
	frameCount = 0;

	modelX = 0.0f;
	modelY = 0.0f;
	modelDx = 0.0f;
	modelDy = 0.0f;
	angle = 0.0f;
	angleToPlus = 3.0f;

	// initializes with a triangle buffer
	d3dDev->CreateVertexBuffer(3 * sizeof(CUSTOM_VERTEX), 0, CUSTOM_FVF, D3DPOOL_MANAGED, &verticesBuffer, NULL);
	assert(verticesBuffer);
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

void Game::fini()
{
	if (verticesBuffer != NULL) {
		verticesBuffer->Release();
		verticesBuffer = NULL;
	}
}

void Game::fps()
{
	frameCount++;
	ULONGLONG timeUsage = GetTickCount() - timeStart;
	if (timeUsage > 1000) {
		ULONGLONG fps = frameCount * 1000 / timeUsage;
		timeStart = GetTickCount();
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		OutputDebugStringA("SPACE keydown\n");
		return true;
	}
	else if (key == VK_RETURN) {
		// space
		OutputDebugStringA("ENTER keydown\n");
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		OutputDebugStringA("W -or- UP keydown\n");
		modelDy = -0.1f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringA("S -or- DOWN keydown\n");
		modelDy = 0.1f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringA("A -or- LEFT keydown\n");
		modelDx = -0.1f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringA("D -or- RIGHT keydown\n");
		modelDx = 0.1f;
		return true;
	}
	return false;
}

void Game::render()
{
	d3dDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	d3dDev->BeginScene();
	d3dDev->SetFVF(CUSTOM_FVF);

	D3DXMATRIX matrixModel;
	D3DXMatrixTranslation(&matrixModel, modelX + modelDx, modelY + modelDy, 0);
	modelX += modelDx;
	modelY += modelDy;
	modelDx = 0.0f;
	modelDy = 0.0f;
	D3DXMATRIX matrixRotateY;
	D3DXMatrixRotationY(&matrixRotateY, D3DXToRadian(angle));
	angle += angleToPlus;
	if (angle > 89.0f || angle < -89.0f)
		angleToPlus = -angleToPlus;
	d3dDev->SetTransform(D3DTS_WORLD, &(matrixRotateY * matrixModel));

	D3DXMATRIX matrixView;
	D3DXMatrixLookAtLH(&matrixView,
		&D3DXVECTOR3(0.0f, 0.0f, -3.0f),  // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),   // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction
	d3dDev->SetTransform(D3DTS_VIEW, &matrixView);

	D3DXMATRIX matrixProjection;
	D3DXMatrixPerspectiveFovLH(&matrixProjection,
		D3DXToRadian(45),  // the horizontal field of view
		1.3333f,           // aspect ratio
		1.0f,              // the near view-plane
		10.0f);            // the far view-plane
	d3dDev->SetTransform(D3DTS_PROJECTION, &matrixProjection);

	// draws vertex buffer to display
	d3dDev->SetStreamSource(0, verticesBuffer, 0, sizeof(CUSTOM_VERTEX));
	d3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3dDev->EndScene();
	d3dDev->Present(NULL, NULL, NULL, NULL);

	fps();
}

