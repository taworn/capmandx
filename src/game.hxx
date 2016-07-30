#ifndef GAME_HXX
#define GAME_HXX

#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX {
	float x, y, z;
	DWORD color;
};

class Scene;

class Game
{
public:
	static Game* instance() { return singleton; }

	~Game();
	Game(IDirect3DDevice9 *dev);

	IDirect3DDevice9* getDevice() { return d3dDev; }
	Scene* currentScene() { return scene; }

	void changeScene(Scene *newScene);

private:
	IDirect3DDevice9 *d3dDev;
	Scene *scene;

	static Game *singleton;
};

#endif // GAME_HXX

