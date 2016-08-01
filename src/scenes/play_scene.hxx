#ifndef SCENES_PLAY_SCENE_HXX
#define SCENES_PLAY_SCENE_HXX

class PlayScene : public Scene
{
public:
	virtual ~PlayScene();
	PlayScene();

	virtual void reset();

protected:
	void init();
	void fini();

private:
	float modelX;
	float modelY;
	float modelDx;
	float modelDy;
	float angle;
	float angleToPlus;
	IDirect3DVertexBuffer9 *verticesBuffer;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_PLAY_SCENE_HXX

