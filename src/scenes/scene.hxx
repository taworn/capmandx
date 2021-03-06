/**
 * @file scene.hxx
 * @desc Game scene header.
 */
#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

/**
 * A base game scene.
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
	 * Initializes a game scene.
	 */
	virtual void init();

	/**
	 * Uninitializes a game scene.
	 */
	virtual void fini();

protected:
	/**
	 * Computes current frames per second.
	 */
	void computeFPS();

	int getFPS() const { return fps; }
	RECT getScreenRect() const { return screenRect; }

private:
	RECT screenRect;
	int fps;
	int frameCount;
	ULONGLONG timeStart;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	/**
	 * Called when window is activated/deactivated.
	 */
	virtual void handleActivate(HWND hwnd, bool active);

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

