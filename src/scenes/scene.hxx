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

	IDirect3DDevice9* getDevice() const { return device; }
	RECT getScreenRect() const { return screenRect; }
	ID3DXFont* getSmallFont() { return smallFont; }
	ID3DXFont* getNormalFont() { return normalFont; }
	ID3DXFont* getBigFont() { return bigFont; }
	int getFPS() const { return fps; }

private:
	IDirect3DDevice9 *device;
	RECT screenRect;
	ID3DXFont* smallFont;
	ID3DXFont* normalFont;
	ID3DXFont* bigFont;
	int fps;
	int frameCount;
	ULONGLONG timeStart;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_SCENE_HXX

