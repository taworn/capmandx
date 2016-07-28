#ifndef RENDER_HXX
#define RENDER_HXX

#ifdef __cplusplus
extern "C" {
#endif

#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX {
	float x, y, z;
	DWORD color;
};

bool RenderInit();
void RenderUninit();
void Render();

#ifdef __cplusplus
}
#endif

#endif // RENDER_HXX

