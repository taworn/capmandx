/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

#include <vector>

/**
 * A map class.
 */
class Map
{
public:
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

	void setPacmanPosition(float *returnX, float *returnY);

	bool canPass(float x, float *dx, float y, float *dy);

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



	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

