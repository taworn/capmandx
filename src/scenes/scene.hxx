/**
 * @file scene.hxx
 * @desc Game scene header.
 */
#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

/**
 * A single game scene.
 */
class Scene
{
public:
	/**
	 * Destructs a game scene.
	 */
	virtual ~Scene();

	/**
	 * Constructs a game scene.
	 */
	Scene();

	/**
	 * Resets this scene.
	 */
	virtual void reset();

protected:
	/**
	 * Initializes a game scene.
	 */
	void init();

	/**
	 * Uninitializes a game scene.
	 */
	void fini();

	/**
	 * Computes current frames per second.
	 */
	void computeFPS();

	IDirect3DDevice9* getDevice() const { return device; }
	RECT getScreenRect() const { return screenRect; }
	ID3DXFont* getSmallFont() { return smallFont; }
	ID3DXFont* getNormalFont() { return normalFont; }
	ID3DXFont* getBigFont() { return bigFont; }
	int getFPS() const { return fps; }

private:
	IDirect3DDevice9 *device;
	RECT screenRect;
	ID3DXFont *smallFont;
	ID3DXFont *normalFont;
	ID3DXFont *bigFont;
	int fps;
	int frameCount;
	ULONGLONG timeStart;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	/**
	 * Called when user press keyboard.
	 */
	virtual bool handleKey(HWND hwnd, WPARAM key);

	/**
	 * Called every render frame.
	 */
	virtual void render();
};

#endif // SCENES_SCENE_HXX

