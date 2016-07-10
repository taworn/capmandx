#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"
#include "render.hxx"

static LPDIRECT3DVERTEXBUFFER9 verticesBuffer = NULL;
static float angle = 0.0f;
static float angleToPlus = 1.0f;
static DWORD lastTick = 0;

bool RenderInit()
{
	// initializes with a triangle buffer
	d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOM_VERTEX), 0, CUSTOM_FVF, D3DPOOL_MANAGED, &verticesBuffer, NULL);
	if (verticesBuffer == NULL)
		return false;

	CUSTOM_VERTEX verticesData[] = {
	    { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
	    { 0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
	    { 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
	};
	void *p;
	verticesBuffer->Lock(0, 0, (void**)&p, 0);
	memcpy(p, verticesData, sizeof(verticesData));
	verticesBuffer->Unlock();

	lastTick = GetTickCount();
	return true;
}

void RenderUninit()
{
	if (verticesBuffer != NULL) {
		verticesBuffer->Release();
		verticesBuffer = NULL;
	}
}

void Render()
{
	if (GetTickCount() - lastTick > 10) {
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
		d3ddev->BeginScene();
		d3ddev->SetFVF(CUSTOM_FVF);

		D3DXMATRIX matrixRotateY;
		D3DXMatrixRotationY(&matrixRotateY, D3DXToRadian(angle));
		angle += angleToPlus;
		if (angle > 89.0f || angle < -89.0f)
			angleToPlus = -angleToPlus;
		d3ddev->SetTransform(D3DTS_WORLD, &matrixRotateY);

		D3DXMATRIX matrixView;
		D3DXMatrixLookAtLH(&matrixView,
			&D3DXVECTOR3(0.0f, 0.0f, -3.0f),  // the camera position
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),   // the look-at position
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // the up direction
		d3ddev->SetTransform(D3DTS_VIEW, &matrixView);

		D3DXMATRIX matrixProjection;
		D3DXMatrixPerspectiveFovLH(&matrixProjection,
			D3DXToRadian(45),  // the horizontal field of view
			1.3333f,           // aspect ratio
			1.0f,              // the near view-plane
			10.0f);            // the far view-plane
		d3ddev->SetTransform(D3DTS_PROJECTION, &matrixProjection);

		// draws vertex buffer to display
		d3ddev->SetStreamSource(0, verticesBuffer, 0, sizeof(CUSTOM_VERTEX));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
		lastTick = GetTickCount();
	}
}

