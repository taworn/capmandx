/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

#include <vector>

class Movable;

/**
 * A map class.
 */
class Map
{
public:
	static const int MOVE_LEFT = 1;
	static const int MOVE_RIGHT = 2;
	static const int MOVE_UP = 4;
	static const int MOVE_DOWN = 8;

	/**
	 * Destructs the map.
	 */
	~Map();

	/**
	 * Constructs the map.
	 */
	Map();

	/**
	 * Loads map data.
	 */
	bool load();

	/**
	 * Draws map
	 */
	void draw(IDirect3DDevice9 *device, Sprite *sprite);

	/**
	 * Checks if direction is can be pass.
	 */
	bool canMove(Movable *movable, int direction, POINT *p, POINTFLOAT *pf);

	/**
	 * Checks 4-directions which ways can move or not.
	 * @return Bit flags in order: left, right, up, down.
	 */
	int canPreviewMove(Movable *movable);

	void setPacmanPosition(POINT *p, POINTFLOAT *pf);
	void setDivoPosition(POINT *p, POINTFLOAT *pf);

private:
	union MapData {
		struct {
			char image;
			char block;
			char eat;
			char special;
		};
		int data;
	};
	int width, height;
	MapData *mapData;

	std::vector<float> horzBounds;
	std::vector<float> horzPoints;
	std::vector<float> vertBounds;
	std::vector<float> vertPoints;

	POINT startPacman;
	POINT startDivo;

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

