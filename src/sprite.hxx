/**
 * @file sprite.hxx
 * @desc Sprite header.
 */
#ifndef SPRITE_HXX
#define SPRITE_HXX

#include <vector>

/**
 * A sprite class.
 */
class Sprite
{
public:
	/**
	 * Destructs a sprite.
	 */
	~Sprite();

	/**
	 * Constructs a sprite.
	 */
	Sprite();

	/**
	 * Initializes a sprite with image.
	 */
	bool init(IDirect3DDevice9 *device, const wchar_t *imageFileName, int sliceHorz, int sliceVert);

	/**
	 * Draws sprite.
	 */
	void draw(IDirect3DDevice9 *device, int imageIndex);

	/**
	 * Gets number of images.
	 */
	int getImageCount() const { return sliceHorz * sliceVert; }

private:
	struct VERTEX {
		float x, y, z;
		float u, v;
	};

	IDirect3DVertexBuffer9 *verticesBuffer;
	IDirect3DTexture9 *texture;
	std::vector<float> uData;
	std::vector<float> vData;
	int sliceHorz;
	int sliceVert;

	Sprite(const Sprite&);
	Sprite& operator=(const Sprite&);
};

#endif // SPRITE_HXX

