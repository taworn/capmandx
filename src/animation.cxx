/**
 * @file animation.cxx
 * @desc Animation module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "sprite.hxx"
#include "animation.hxx"

Animation::~Animation()
{
}

Animation::Animation(Sprite *s) : sprite(s), plays(), currentPlaying(-1), currentImage(0)
{
	assert(sprite);
	timeStart = GetTickCount();
}

void Animation::setSprite(Sprite *s)
{
	sprite = s;
	assert(sprite);
	use(currentPlaying);
}

void Animation::add(int number, int start, int end, int time)
{
	assert(number >= 0 && number < PLAYING_MAX);
	plays[number].start = start;
	plays[number].end = end;
	plays[number].time = time;
}

void Animation::use(int number)
{
	assert(number >= 0 && number < PLAYING_MAX);
	if (number != currentPlaying) {
		currentPlaying = number;
		currentImage = plays[number].start;
		timeStart = GetTickCount();
	}
}

void Animation::draw(IDirect3DDevice9 *device)
{
	sprite->draw(device, currentImage);

	ULONGLONG usage = GetTickCount() - timeStart;
	if (usage > plays[currentPlaying].time) {
		currentImage++;
		if (currentImage >= plays[currentPlaying].end)
			currentImage = plays[currentPlaying].start;
		timeStart = GetTickCount();
	}
}

