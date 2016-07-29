#ifndef GAME_HXX
#define GAME_HXX

#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX {
	float x, y, z;
	DWORD color;
};

class Game
{
private:
	IDirect3DDevice9 *d3dDev;
	ULONGLONG timeStart;
	int frameCount;

	float modelX;
	float modelY;
	float modelDx;
	float modelDy;
	float angle;
	float angleToPlus;
	IDirect3DVertexBuffer9 *verticesBuffer;

	void init();
	void fini();
	void fps();

public:
	Game(IDirect3DDevice9 *dev) : d3dDev(dev)
	{
		init();
	}

	~Game()
	{
		fini();
	}

	bool handleKey(HWND hwnd, WPARAM key);
	void render();
};

#endif // GAME_HXX

