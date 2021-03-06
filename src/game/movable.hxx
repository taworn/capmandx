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
	static const int MOVE_LEFT = 1;
	static const int MOVE_RIGHT = 2;
	static const int MOVE_UP = 4;
	static const int MOVE_DOWN = 8;

	static const int ACTION_LEFT = 0;
	static const int ACTION_RIGHT = 1;
	static const int ACTION_UP = 2;
	static const int ACTION_DOWN = 3;
	static const int ACTION_REVERSE_LEFT = 4;
	static const int ACTION_REVERSE_RIGHT = 5;
	static const int ACTION_REVERSE_UP = 6;
	static const int ACTION_REVERSE_DOWN = 7;
	static const int ACTION_DEAD_LEFT = 8;
	static const int ACTION_DEAD_RIGHT = 9;
	static const int ACTION_DEAD_UP = 10;
	static const int ACTION_DEAD_DOWN = 11;

	static const int TIME_PER_ANI_FRAME = 250;

	/**
	 * Destructs the movable.
	 */
	~Movable();

	/**
	 * Constructs the movable.
	 */
	Movable();

	/**
	 * Moves by direction, use constant Map::MOVE_*.
	 */
	virtual void move(int direction);

	/**
	 * Moves to (x, y) directly.
	 */
	virtual void moveDirect(POINT *p, POINTFLOAT *pf);

	/**
	 * Chooses next action.  This function is called after play() is completed.
	 */
	virtual void nextAction();

	/**
	 * Plays animation after call move() or moveDirectly().
	 */
	virtual void play(ULONGLONG timeUsed);

	/**
	 * Kills this movable.  Inherit class should derived this function.
	 */
	virtual void kill() { dead = true; }

	/**
	 * Relife your life.
	 */
	virtual void relife() { dead = false; }

	/**
	 * Checks whether movable is dead.
	 */
	bool isDead() const { return dead; }

	/**
	 * Checks whether movable is busing or idling.
	 */
	bool isIdle() const { return !dead && !animating; }

	/**
	 * Gets X-coordinates in integer.
	 */
	int getX() const { return point.x; }

	/**
	 * Gets Y-coordinates in integer.
	 */
	int getY() const { return point.y; }

	/**
	 * Gets X-coordinates in float.
	 */
	float getCurrentX() const { return animation.getCurrentX(); }

	/**
	 * Gets Y-coordinates in float.
	 */
	float getCurrentY() const { return animation.getCurrentY(); }

	/**
	 * Sets map.  Used to bind Movable with Map.
	 */
	virtual void setMap(Map *map)
	{
		assert(map);
		this->map = map;
	}

	/**
	 * Draws movable.
	 */
	void draw(IDirect3DDevice9 *device, Sprite *sprite, D3DXMATRIX *matrixScale, POINTFLOAT *scaleUp);

protected:
	/**
	 * Chooses which action after animation is completed.
	 */
	virtual int decision(int moveDirection);

	/**
	 * Sets start position.
	 */
	void setXY(int x, int y)
	{
		point.x = x;
		point.y = y;
	}

	/**
	 * Sets times in case of you want difference times table.
	 */
	void setTimes(ULONGLONG timePerMove, ULONGLONG timePerDead)
	{
		this->timePerMove = timePerMove;
		this->timePerDead = timePerDead;
	}

	Animation* getAnimation() { return &animation; }
	Map* getMap() { return map; }

private:
	POINT point;  ///< Point (x, y) in integer.

	// state flags
	bool dead;       ///< Alive (false) or dead (true).
	bool animating;  ///< Animating flags.

	// moving animation
	float distanceX, distanceY;
	float targetX, targetY;
	int currentDirection;
	int nextDirection;
	ULONGLONG timePerMove;
	ULONGLONG timePerDead;
	ULONGLONG timePerDistance;
	ULONGLONG timeUsed;
	Animation animation;

	Map *map;

	Movable(const Movable&);
	Movable& operator=(const Movable&);
};

#endif // GAME_MOVABLE_HXX

