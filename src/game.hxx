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

	bool deviceReset();
	void changeScene(Scene *newScene);

	IDirect3DDevice9* getDevice() const { return d3dDev; }
	Scene* currentScene() const { return scene; }

	bool handleKey(HWND hwnd, WPARAM key);
	void render();

private:
	IDirect3DDevice9 *d3dDev;
	Scene *scene;

	static Game *singleton;
};

#endif // GAME_HXX

