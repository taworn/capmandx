#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

class Scene
{
public:
	virtual ~Scene();
	Scene();

	virtual void reset();

protected:
	void init();
	void fini();

	void computeFPS();
	void drawFPS();

	IDirect3DDevice9* getDevice() const { return device; }
	RECT getScreenRect() const { return screenRect; }
	ULONGLONG getFPS() const { return fps; }

private:
	IDirect3DDevice9 *device;
	RECT screenRect;
	int frameCount;
	ULONGLONG fps;
	ULONGLONG timeStart;
	ID3DXFont *fpsFont;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_SCENE_HXX

