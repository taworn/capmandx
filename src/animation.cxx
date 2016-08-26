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

Animation::Animation()
	: plays(), currentPlaying(-1), currentImage(0), ending(false)
	, currentX(0), currentY(0), velocityX(0), velocityY(0)
{
	timeStart = GetTickCount();
}

void Animation::add(int number, int start, int end, int onEnd, int time)
{
	assert(number >= 0 && number < PLAYING_MAX);
	plays[number].start = start;
	plays[number].end = end;
	plays[number].onEnd = onEnd;
	plays[number].time = time;
}

void Animation::use(int number)
{
	assert(number >= 0 && number < PLAYING_MAX);
	if (number != currentPlaying) {
		currentPlaying = number;
		currentImage = plays[number].start;
		ending = false;
		timeStart = GetTickCount();
	}
}

void Animation::draw(IDirect3DDevice9 *device, Sprite *sprite)
{
	if (!ending) {
		sprite->draw(device, currentImage);

		ULONGLONG usage = GetTickCount() - timeStart;
		if (usage > (ULONGLONG)plays[currentPlaying].time) {
			currentImage++;
			if (currentImage >= plays[currentPlaying].end) {
				switch (plays[currentPlaying].onEnd) {
				default:
				case ON_END_CONTINUE:
					currentImage = plays[currentPlaying].start;
					break;
				case ON_END_KEEP_LAST_FRAME:
					currentImage--;
					ending = true;
					break;
				case ON_END_HIDDEN:
					ending = true;
					break;
				}
			}
			timeStart = GetTickCount();
		}
	}
	else {
		if (plays[currentPlaying].onEnd == ON_END_KEEP_LAST_FRAME)
			sprite->draw(device, currentImage);
	}
}

