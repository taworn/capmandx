#ifndef SCENES_TITLE_SCENE_HXX
#define SCENES_TITLE_SCENE_HXX

class TitleScene : public Scene
{
public:
	virtual ~TitleScene();
	TitleScene();

	virtual void reset();

protected:
	void init();
	void fini();

private:
	ID3DXFont *titleFont;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

