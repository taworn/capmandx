/**
 * @file play_scene.hxx
 * @desc Playing scene header.
 */
#ifndef SCENES_PLAY_SCENE_HXX
#define SCENES_PLAY_SCENE_HXX

/**
 * Playing game scene.
 */
class PlayScene : public Scene
{
public:
	virtual ~PlayScene();
	PlayScene();

	virtual void init();
	virtual void fini();

private:
	Sprite *sprite;
	Animation *aniHero;
	Animation *aniDivoes[4];

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_PLAY_SCENE_HXX

