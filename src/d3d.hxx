#ifndef D3D_HXX
#define D3D_HXX

#ifdef __cplusplus
extern "C" {
#endif

extern bool active;
extern IDirect3D9 *d3d;
extern IDirect3DDevice9 *d3dDev;
extern D3DPRESENT_PARAMETERS d3dpp;

bool D3DInit(HWND hwnd);
bool D3DReset();
void D3DUninit();

#ifdef __cplusplus
}
#endif

#endif // D3D_HXX

