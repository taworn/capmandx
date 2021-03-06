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

	virtual void init();
	virtual void fini();

private:
	ID3DXFont *titleFont;
	Sprite *spritePacman;
	Sprite *spriteUI;
	Animation *aniDivo;
	Animation *aniHero;
	float modelX;
	int menuIndex;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

