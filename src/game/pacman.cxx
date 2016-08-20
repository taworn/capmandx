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
}

void Pacman::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->setPacmanPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

