/**
 * @file pacman.cxx
 * @desc Pacman module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "common.hxx"

Pacman::~Pacman()
{
}

Pacman::Pacman()
{
	timePerDistance = 200;
	add(0, 0, 2, TIME_PER_ANI_FRAME);
	add(1, 2, 4, TIME_PER_ANI_FRAME);
	add(2, 4, 6, TIME_PER_ANI_FRAME);
	add(3, 6, 8, TIME_PER_ANI_FRAME);
	use(0);
}

void Pacman::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->setPacmanPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

