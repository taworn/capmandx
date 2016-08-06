/**
 * @file game.hxx
 * @desc Game engine header.
 */
#ifndef GAME_HXX
#define GAME_HXX

#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOM_VERTEX {
	float x, y, z;
	DWORD color;
};

#define SCENE_DEFAULT 0
#define SCENE_TITLE 1
#define SCENE_PLAY 2

class Scene;

/**
 * A simple game engine class.
 */
class Game
{
public:
	static Game* instance() { return singleton; }

	/**
	 * Destructs the game engine.
	 */
	~Game();

	/**
	 * Constructs the game engine.
	 */
	Game(IDirect3DDevice9 *dev);

	/**
	 * Resets current scene.
	 */
	bool deviceReset();

	/**
	 * Changes the new scene.
	 * @param sceneId A scene identifier, look at SCENE_*.
	 */
	void changeScene(int sceneId);

	/**
	 * Called when user press keyboard.
	 */
	bool handleKey(HWND hwnd, WPARAM key);

	/**
	 * Called every render frame.
	 */
	void render();

	IDirect3DDevice9* getDevice() const { return d3dDev; }
	ID3DXFont* getSmallFont() { return smallFont; }
	ID3DXFont* getNormalFont() { return normalFont; }
	ID3DXFont* getBigFont() { return bigFont; }
	Scene* currentScene() const { return scene; }

private:
	IDirect3DDevice9 *d3dDev;
	ID3DXFont *smallFont;
	ID3DXFont *normalFont;
	ID3DXFont *bigFont;
	Scene *scene;

	static Game *singleton;
};

#endif // GAME_HXX

