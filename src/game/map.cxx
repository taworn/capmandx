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
	if (imageData)
		delete[] imageData;
	if (mapData)
		delete[] mapData;
}

Map::Map()
	: width(), height(), mapData(), imageData()
	, horzBounds(), horzPoints(), vertBounds(), vertPoints()
	, startDivo(), startPacman()
{
}

bool Map::load()
{
	const int w = 16;
	const int h = 16;
	width = w;
	height = h;
	mapData = new char[width * height];
	imageData = new int[width * height];

	// for now, emulate first
	char _mapData[w * h] = {
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01,
		0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	};
	int _imageData[w * h] = {
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
		0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x03,
		0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x00, 0x00, 0x03,
		0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	};
	memcpy(mapData, _mapData, sizeof(_mapData));
	memcpy(imageData, _imageData, sizeof(_imageData));

	horzBounds.clear();
	horzBounds.resize(width + 1);
	for (size_t i = 0; i < horzBounds.size(); i++)
		horzBounds[i] = (float)i / (horzBounds.size() - 1) * 2.0f - 1.0f;

	horzPoints.clear();
	horzPoints.resize(width);
	for (size_t i = 0; i < horzPoints.size(); i++)
		horzPoints[i] = (horzBounds[i] + horzBounds[i + 1]) / 2.0f;

	vertBounds.clear();
	vertBounds.resize(height + 1);
	for (size_t i = 0; i < vertBounds.size(); i++)
		vertBounds[i] = (float)(vertBounds.size() - 1 - i) / (vertBounds.size() - 1) * 2.0f - 1.0f;

	vertPoints.clear();
	vertPoints.resize(height);
	for (size_t i = 0; i < vertPoints.size(); i++)
		vertPoints[i] = (vertBounds[i] + vertBounds[i + 1]) / 2.0f;

	startDivo.x = 8;
	startDivo.y = 1;
	startPacman.x = 8;
	startPacman.y = 10;
	return true;
}

void Map::getDivoStartPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startDivo.x;
	p->y = startDivo.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

void Map::getPacmanStartPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startPacman.x;
	p->y = startPacman.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

bool Map::canMove(Movable *movable, int direction, POINT *p, POINTFLOAT *pf)
{
	if (direction == MOVE_LEFT) {
		int current = movable->getX();
		int next = current - 1;
		if (next >= 0) {
			int block = mapData[movable->getY() * width + next] & 0x01;
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
			int block = mapData[movable->getY() * width + next] & 0x01;
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
			int block = mapData[next * width + movable->getX()] & 0x01;
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
			int block = mapData[next * width + movable->getX()] & 0x01;
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

int Map::canPreviewMove(Movable *movable)
{
	int x = movable->getX();
	int y = movable->getY();
	int result = 0;

	// left
	if (x > 0 && !(mapData[y * width + x - 1] & 0x01))
		result |= MOVE_LEFT;
	// right
	if (x < width - 1 && !(mapData[y * width + x + 1] & 0x01))
		result |= MOVE_RIGHT;
	// up
	if (y > 0 && !(mapData[(y - 1) * width + x] & 0x01))
		result |= MOVE_UP;
	// down
	if (y < height - 1 && !(mapData[(y + 1) * width + x] & 0x01))
		result |= MOVE_DOWN;

	return result;
}

void Map::draw(IDirect3DDevice9 *device, Sprite *sprite, D3DXMATRIX *matrixScale)
{
	/*
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			D3DXMATRIX matrixTranslate;
			D3DXMatrixTranslation(&matrixTranslate, horzPoints[i] * scaleUp->x, vertPoints[j] * scaleUp->y, 0);
			device->SetTransform(D3DTS_WORLD, &(matrixTranslate * *matrixScale));
			sprite->draw(device, imageData[j * width + i]);
		}
	}
	*/
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation(&matrixTranslate, 0, 0, 0);
	device->SetTransform(D3DTS_WORLD, &(matrixTranslate * *matrixScale));
	sprite->drawBatch(device, horzBounds, vertBounds, imageData);
}

