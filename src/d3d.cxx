#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3d.hxx"

bool active = false;
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

bool D3DInit(HWND hwnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (d3ddev == NULL) {
		return false;
	}

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return true;
}

void D3DUninit()
{
	if (d3ddev != NULL) {
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d != NULL) {
		d3d->Release();
		d3d = NULL;
	}
}

