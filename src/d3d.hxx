#ifndef D3D_HXX
#define D3D_HXX

#ifdef __cplusplus
extern "C" {
#endif

extern bool active;
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;

bool D3DInit(HWND hwnd);
void D3DUninit();

#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX {
	float x, y, z;
	DWORD color;
};

#ifdef __cplusplus
}
#endif

#endif // D3D_HXX

