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

Divo::~Divo()
{
}

Divo::Divo()
{
}

void Divo::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->setDivoPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

