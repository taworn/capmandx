/**
 * @file d3d.hxx
 * @desc D3D header.
 */
#ifndef D3D_HXX
#define D3D_HXX

#ifdef __cplusplus
extern "C" {
#endif

extern bool active;
extern IDirect3D9 *d3d;
extern IDirect3DDevice9 *d3dDev;
extern D3DPRESENT_PARAMETERS d3dpp;

/**
 * Initializes D3D module.
 */
bool D3DInit(HWND hwnd);

/**
 * Resets and restarts D3D module.
 */
bool D3DReset();

/**
 * Uninitializes D3D module.
 */
void D3DUninit();

#ifdef __cplusplus
}
#endif

#endif // D3D_HXX

