/**
 * @file d3d.cxx
 * @desc D3D module.
 */
#include <windows.h>
#include <d3d9.h>
#include "d3d.hxx"

bool active = false;
IDirect3D9 *d3d = NULL;
IDirect3DDevice9 *d3dDev = NULL;
D3DPRESENT_PARAMETERS d3dpp = { 0 };

bool D3DInit(HWND hwnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	d3dpp = { 0 };
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDev);
	if (d3dDev == NULL) {
		return false;
	}

	d3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//d3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//d3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return true;
}

bool D3DReset()
{
	HRESULT hr = d3dDev->Reset(&d3dpp);
	if (SUCCEEDED(hr)) {
		d3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		return true;
	}
	return false;
}

void D3DUninit()
{
	if (d3dDev != NULL) {
		d3dDev->Release();
		d3dDev = NULL;
	}
	if (d3d != NULL) {
		d3d->Release();
		d3d = NULL;
	}
}

