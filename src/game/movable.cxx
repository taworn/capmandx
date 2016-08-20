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

void Movable::move(int direction)
{
	POINTFLOAT pf;

	if (direction == Map::MOVE_LEFT) {
		animation.use(0);
		if (map->canMove(this, direction, &point, &pf)) {
			BOOST_LOG_TRIVIAL(debug) << "Move to left";
			animation.moveTo(pf.x, pf.y);
		}
	}
	else if (direction == Map::MOVE_RIGHT) {
		animation.use(1);
		if (map->canMove(this, direction, &point, &pf)) {
			BOOST_LOG_TRIVIAL(debug) << "Move to right";
			animation.moveTo(pf.x, pf.y);
		}
	}
	else if (direction == Map::MOVE_UP) {
		animation.use(2);
		if (map->canMove(this, direction, &point, &pf)) {
			BOOST_LOG_TRIVIAL(debug) << "Move to up";
			animation.moveTo(pf.x, pf.y);
		}
	}
	else if (direction == Map::MOVE_DOWN) {
		animation.use(3);
		if (map->canMove(this, direction, &point, &pf)) {
			BOOST_LOG_TRIVIAL(debug) << "Move to down";
			animation.moveTo(pf.x, pf.y);
		}
	}
}

void Movable::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->setPacmanPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

