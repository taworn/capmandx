/**
 * @file map.cxx
 * @desc Map module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "common.hxx"

Map::~Map()
{
	if (mapData)
		delete[] mapData;
}

Map::Map() : width(), height(), mapData()
{
}

bool Map::load()
{
	const int w = 16;
	const int h = 16;
	width = w;
	height = h;
	mapData = new MapData[width * height];

	// for now, emulate first
	int _mapData[w * h] = {
		0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103,
		0x0103, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0103, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0103,
		0x0103, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0103,
		0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103, 0x0103,
	};
	memcpy(mapData, _mapData, sizeof(_mapData));

	horzBounds.clear();
	horzBounds.resize(width + 1);
	for (int i = 0; i < horzBounds.size(); i++)
		horzBounds[i] = (float)i / (horzBounds.size() - 1) * 2.0f - 1.0f;

	horzPoints.clear();
	horzPoints.resize(width);
	for (int i = 0; i < horzPoints.size(); i++)
		horzPoints[i] = (horzBounds[i] + horzBounds[i + 1]) / 2.0f;

	vertBounds.clear();
	vertBounds.resize(height + 1);
	for (int i = 0; i < vertBounds.size(); i++)
		vertBounds[i] = (float)(vertBounds.size() - 1 - i) / (vertBounds.size() - 1) * 2.0f - 1.0f;

	vertPoints.clear();
	vertPoints.resize(height);
	for (int i = 0; i < vertPoints.size(); i++)
		vertPoints[i] = (vertBounds[i] + vertBounds[i + 1]) / 2.0f;

	startPacman.x = 7;
	startPacman.y = 10;
	startDivo.x = 13;
	startDivo.y = 3;
	return true;
}

void Map::draw(IDirect3DDevice9 *device, Sprite *sprite)
{
	D3DXMATRIX matrixScale;
	D3DXMatrixScaling(&matrixScale, 0.0625f, 0.0625f, 1.0f);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			D3DXMATRIX matrixTranslate;
			D3DXMatrixTranslation(&matrixTranslate, horzPoints[i] * 16, vertPoints[j] * 16, 0);
			device->SetTransform(D3DTS_WORLD, &(matrixTranslate * matrixScale));
			sprite->draw(device, (int)mapData[j * width + i].image);
		}
	}
}

void Map::setPacmanPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startPacman.x;
	p->y = startPacman.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

void Map::setDivoPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startDivo.x;
	p->y = startDivo.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

bool Map::canMove(Movable *movable, int direction, POINT *p, POINTFLOAT *pf)
{
	if (direction == MOVE_LEFT) {
		int current = movable->getX();
		int next = current - 1;
		if (next >= 0) {
			int block = mapData[movable->getY() * width + next].block;
			if (!block) {
				p->x = next;
				p->y = movable->getY();
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == MOVE_RIGHT) {
		int current = movable->getX();
		int next = current + 1;
		if (next < width) {
			int block = mapData[movable->getY() * width + next].block;
			if (!block) {
				p->x = next;
				p->y = movable->getY();
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == MOVE_UP) {
		int current = movable->getY();
		int next = current - 1;
		if (next >= 0) {
			int block = mapData[next * width + movable->getX()].block;
			if (!block) {
				p->x = movable->getX();
				p->y = next;
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == MOVE_DOWN) {
		int current = movable->getY();
		int next = current + 1;
		if (next < height) {
			int block = mapData[next * width + movable->getX()].block;
			if (!block) {
				p->x = movable->getX();
				p->y = next;
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}

	return false;
}

