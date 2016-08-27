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
	, dead(false)
	, animating(false)
	, distanceX(0.0f), distanceY(0.0f)
	, targetX(0.0f), targetY(0.0f)
	, currentDirection(0), nextDirection(0)
	, timePerMove(250), timePerDead(750)
	, timePerDistance(0), timeUsed(0)
	, animation()
	, map(NULL)
{
}

void Movable::move(int direction)
{
	if (!dead) {
		if (!animating) {
			POINTFLOAT pf;

			if (!GameData::instance()->isReverseMode()) {
				if (direction == MOVE_LEFT)
					animation.use(ACTION_LEFT);
				else if (direction == MOVE_RIGHT)
					animation.use(ACTION_RIGHT);
				else if (direction == MOVE_UP)
					animation.use(ACTION_UP);
				else if (direction == MOVE_DOWN)
					animation.use(ACTION_DOWN);
			}
			else {
				if (direction == MOVE_LEFT)
					animation.use(ACTION_REVERSE_LEFT);
				else if (direction == MOVE_RIGHT)
					animation.use(ACTION_REVERSE_RIGHT);
				else if (direction == MOVE_UP)
					animation.use(ACTION_REVERSE_UP);
				else if (direction == MOVE_DOWN)
					animation.use(ACTION_REVERSE_DOWN);
			}

			if (map->canMove(this, direction, &point, &pf)) {
				distanceX = pf.x - animation.getCurrentX();
				distanceY = pf.y - animation.getCurrentY();
				targetX = pf.x;
				targetY = pf.y;
				currentDirection = direction;
				nextDirection = direction;
				timePerDistance = timePerMove;
				timeUsed = 0;
				animating = true;
			}
		}
		else {
			// for Pacman, use this for controller
			nextDirection = direction;
		}
	}
}

void Movable::moveDirect(POINT *p, POINTFLOAT *pf)
{
	distanceX = pf->x - animation.getCurrentX();
	distanceY = pf->y - animation.getCurrentY();
	targetX = pf->x;
	targetY = pf->y;
	timePerDistance = timePerDead;
	timeUsed = 0;
	animating = true;
}

void Movable::nextAction()
{
	if (!dead) {
		move(decision(nextDirection));
	}
}

void Movable::play(ULONGLONG timeUsed)
{
	if (animating) {
		if (this->timeUsed + timeUsed < timePerDistance) {
			float dx = timeUsed * distanceX / timePerDistance;
			float dy = timeUsed * distanceY / timePerDistance;
			animation.moveBy(dx, dy);
			this->timeUsed += timeUsed;
		}
		else {
			animation.moveTo(targetX, targetY);
			animating = false;
			nextAction();
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

int Movable::decision(int moveDirection)
{
	return moveDirection;
}

