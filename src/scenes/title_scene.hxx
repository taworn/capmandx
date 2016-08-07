/**
 * @file title_scene.hxx
 * @desc Title scene header.
 */
#ifndef SCENES_TITLE_SCENE_HXX
#define SCENES_TITLE_SCENE_HXX

/**
 * Title game scene.
 */
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
	IDirect3DTexture9 *image;
	IDirect3DVertexBuffer9 *vertexBuffer;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

