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

Movable::Movable()
	: point()
	, walking()
	, distance(0.0f), target(0.0f)
	, currentDirection(0), nextDirection(0)
	, timePerDistance(350), timeUsed(0)
	, animation()
	, map()
{
}

void Movable::move(int direction)
{
	if (!walking) {
		POINTFLOAT pf;
		if (direction == Map::MOVE_LEFT) {
			animation.use(0);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = animation.getCurrentX() - pf.x;
				target = pf.x;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_RIGHT) {
			animation.use(1);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = pf.x - animation.getCurrentX();
				target = pf.x;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_UP) {
			animation.use(2);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = pf.y - animation.getCurrentY();
				target = pf.y;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_DOWN) {
			animation.use(3);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = animation.getCurrentY() - pf.y;
				target = pf.y;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
	}
	else {
		nextDirection = direction;
	}
}

void Movable::nextMove()
{
	move(nextDirection);
}

void Movable::play(ULONGLONG timeUsed)
{
	if (walking) {
		if (currentDirection == Map::MOVE_LEFT) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(-d, 0);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(target, animation.getCurrentY());
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_RIGHT) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(d, 0);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(target, animation.getCurrentY());
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_UP) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(0, d);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(animation.getCurrentX(), target);
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_DOWN) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(0, -d);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(animation.getCurrentX(), target);
				walking = false;
				nextMove();
			}
		}
	}
}

void Movable::draw(IDirect3DDevice9 *device, Sprite *sprite, D3DXMATRIX *matrixScale, POINTFLOAT *scaleUp)
{
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, getCurrentX() * scaleUp->x, getCurrentY() * scaleUp->y, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * *matrixScale));
	animation.draw(device, sprite);
}

