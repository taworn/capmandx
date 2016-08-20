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
	static const int TIME_PER_ANI_FRAME = 250;

	/**
	 * Destructs the movable.
	 */
	~Movable();

	/**
	 * Constructs the movable.
	 */
	Movable();

	// Animation class' functions
	void add(int number, int start, int end, int time) { animation.add(number, start, end, time); }
	void use(int number) { animation.use(number); }
	void draw(IDirect3DDevice9 *device, Sprite *sprite) { animation.draw(device, sprite); }
	float getCurrentX() const { return animation.getCurrentX(); }
	float getCurrentY() const { return animation.getCurrentY(); }
	float getVelocityX() const { return animation.getVelocityX(); }
	float getVelocityY() const { return animation.getVelocityY(); }
	void playFrame(bool enableX, bool enableY) { animation.playFrame(enableX, enableY); }

	// new functions
	int getX() const { return point.x; }
	int getY() const { return point.y; }
	void move(int direction);
	void play(ULONGLONG timeUsed);
	virtual void nextMove();
	virtual void setMap(Map *map) = 0;

protected:
	POINT point;
	bool walking;
	float distance;
	float target;
	int currentDirection;
	int nextDirection;
	ULONGLONG timePerDistance;
	ULONGLONG timeUsed;

	Animation animation;
	Map *map;

private:
	Movable(const Movable&);
	Movable& operator=(const Movable&);
};

#endif // GAME_MOVABLE_HXX

