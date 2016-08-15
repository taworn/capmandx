/**
 * @file animation.hxx
 * @desc Animation header.
 */
#ifndef ANIMATION_HXX
#define ANIMATION_HXX

#include <vector>

/**
 * An animation class.
 */
class Animation
{
public:
	/**
	 * Destructs an animation.
	 */
	~Animation();

	/**
	 * Constructs an animation.
	 */
	Animation();

	/**
	 * Initializes an animation with image.
	 */
	bool init(IDirect3DDevice9 *device, const wchar_t *imageFileName, int sliceWidth, int sliceHeight, ULONGLONG timeFrame);

	/**
	 * Sets parameters.
	 */
	void setParameters(int spriteStart, int spriteStop, int timeFrame);

	/**
	 * Draws animation.
	 */
	void draw(IDirect3DDevice9 *device);

private:
	IDirect3DVertexBuffer9 *verticesBuffer;
	IDirect3DTexture9 *texture;
	std::vector<float> uData;
	std::vector<float> vData;
	int sliceWidth, sliceHeight;
	int spriteStart, spriteStop;
	int currentIndex;
	ULONGLONG timeFrame;
	ULONGLONG timeStart;

	Animation(const Animation&);
	Animation& operator=(const Animation&);
};

#endif // ANIMATION_HXX

