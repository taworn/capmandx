/**
 * @file movable.cxx
 * @desc Movable module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "common.hxx"

Movable::~Movable()
{
}

Movable::Movable() : animation()
{
}

void Movable::setMap(Map *map)
{
	assert(map);
	this->map = map;
	
	float x, y;
	map->setPacmanPosition(&x, &y);
	animation.moveTo(x, y);
}

void Movable::draw(IDirect3DDevice9 *device, Sprite *sprite)
{
	animation.draw(device, sprite);
}

void Movable::moveTo(float x, float y)
{

	animation.moveTo(x, y);
}

void Movable::setVelocity(float x, float y)
{
	if (map->canPass(animation.getCurrentX(), &x, animation.getCurrentY(), &y))
		animation.setVelocity(x, y);
}

