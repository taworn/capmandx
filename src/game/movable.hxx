/**
 * @file movable.hxx
 * @desc Movable header.
 */
#ifndef GAME_MOVABLE_HXX
#define GAME_MOVABLE_HXX

class Sprite;
class Animation;
class Map;

/**
 * A movable class.
 */
class Movable
{
public:
	/**
	 * Destructs the movable.
	 */
	~Movable();

	/**
	 * Constructs the movable.
	 */
	Movable();

	void setMap(Map *map);

	void add(int number, int start, int end, int time) { animation.add(number, start, end, time); }
	void use(int number) { animation.use(number); }
	void draw(IDirect3DDevice9 *device, Sprite *sprite);

	float getCurrentX() const { return animation.getCurrentX(); }
	float getCurrentY() const { return animation.getCurrentY(); }
	float getVelocityX() const { return animation.getVelocityX(); }
	float getVelocityY() const { return animation.getVelocityY(); }

	void moveTo(float x, float y);
	void moveBy(float dx, float dy) { animation.moveBy(dx, dy); }
	void setVelocity(float x, float y);
	void playFrame(bool enableX, bool enableY) { animation.playFrame(enableX, enableY); }

private:
	Animation animation;
	Map *map;

	Movable(const Movable&);
	Movable& operator=(const Movable&);
};

#endif // GAME_MOVABLE_HXX

