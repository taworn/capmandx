/**
 * @file divo.cxx
 * @desc Divo module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "common.hxx"
#include <time.h>

Divo::~Divo()
{
}

Divo::Divo()
{
	srand((int)GetTickCount());
}

void Divo::setId(int divoId)
{
	assert(divoId >= 0 && divoId < 4);
	add(0, (divoId + 1) * 8 + 0, (divoId + 1) * 8 + 2, TIME_PER_ANI_FRAME);
	add(1, (divoId + 1) * 8 + 2, (divoId + 1) * 8 + 4, TIME_PER_ANI_FRAME);
	add(2, (divoId + 1) * 8 + 4, (divoId + 1) * 8 + 6, TIME_PER_ANI_FRAME);
	add(3, (divoId + 1) * 8 + 6, (divoId + 1) * 8 + 8, TIME_PER_ANI_FRAME);
	use(0);
}

void Divo::nextMove()
{
	int dirs = map->canPreviewMove(this);
	
	while (1) {
		int random = rand() % 4;
		if (dirs & (1 << random)) {
			nextDirection = random + 1;
			break;
		}
	}

	move(nextDirection);
}

void Divo::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->setDivoPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

