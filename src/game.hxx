/**
 * @file game.hxx
 * @desc Game engine header.
 */
#ifndef GAME_HXX
#define GAME_HXX

#include "sprite.hxx"
#include "animation.hxx"

class Scene;

/**
 * A simple game engine class.
 */
class Game
{
public:
	static Game* instance() { return singleton; }

	static const int SCENE_DEFAULT = 0;
	static const int SCENE_TITLE = 1;
	static const int SCENE_STAGE = 2;
	static const int SCENE_PLAY = 3;
	static const int SCENE_GAMEOVER = 4;
	static const int SCENE_WIN = 5;

	/**
	 * Destructs the game engine.
	 */
	~Game();

	/**
	 * Constructs the game engine.
	 */
	Game(IDirect3DDevice9 *dev);

	/**
	 * Changes the new scene.
	 * @param sceneId A scene identifier, look at SCENE_*.
	 */
	void changeScene(int sceneId);

	/**
	 * Called when window is activated/deactivated.
	 */
	void handleActivate(HWND hwnd, bool active);

	/**
	 * Called when user press keyboard.
	 */
	bool handleKey(HWND hwnd, WPARAM key);

	/**
	 * Called every render frame.
	 */
	void render();

	/**
	 * Draws texture.
	 */
	void draw(IDirect3DTexture9 *image);

	IDirect3DDevice9* getDevice() const { return d3dDev; }
	ID3DXFont* getSmallFont() { return smallFont; }
	ID3DXFont* getNormalFont() { return normalFont; }
	ID3DXFont* getBigFont() { return bigFont; }
	Scene* currentScene() const { return scene; }

private:
	struct TEXTURE_VERTEX {
		float x, y, z;
		float u, v;
	};

	IDirect3DDevice9 *d3dDev;
	ID3DXFont *smallFont;
	ID3DXFont *normalFont;
	ID3DXFont *bigFont;
	IDirect3DVertexBuffer9 *textureVerticesBuffer;
	Scene *scene;
	int nextSceneId;

	/**
	 * Performs real scene switching.
	 */
	void switchScene();

	/**
	 * Initializes game engine.
	 */
	void init();

	/**
	 * Uninitializes game engine.
	 */
	void fini();

	static Game *singleton;
};

#endif // GAME_HXX

