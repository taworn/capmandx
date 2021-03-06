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
	setTimes(150, 1000);
	getAnimation()->add(ACTION_LEFT, 0, 2, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_RIGHT, 2, 4, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_UP, 4, 6, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DOWN, 6, 8, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_LEFT, 0, 2, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_RIGHT, 2, 4, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_UP, 4, 6, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_DOWN, 6, 8, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD_DOWN, 60, 64, Animation::ON_END_HIDDEN, 500);
	getAnimation()->use(ACTION_LEFT);
}

void Pacman::detect()
{
	if (!isDead()) {
		const float RANGE = 0.03125f;
		float x = getCurrentX();
		float y = getCurrentY();
		float left = x - RANGE;
		float top = y + RANGE;
		float right = x + RANGE;
		float bottom = y - RANGE;

		GameData *gameData = GameData::instance();
		int count = gameData->getDivoCount();
		int i = 0;
		std::vector<Divo*> detectedList;
		while (i < count) {
			Divo *divo = gameData->getDivo(i);
			float divoX = divo->getCurrentX();
			float divoY = divo->getCurrentY();

			if (!divo->isDead()) {
				if (left < divoX && top > divoY && divoX < right && divoY > bottom) {
					detectedList.push_back(divo);
				}
			}

			i++;
		}

		for (size_t i = 0; i < detectedList.size(); i++) {
			if (!GameData::instance()->isReverseMode()) {
				detectedList[i]->kill();
				BOOST_LOG_TRIVIAL(debug) << "eat Divo #" << i;
			}
			else {
				kill();
				BOOST_LOG_TRIVIAL(debug) << "Pacman dead";
			}
		}
	}
}

void Pacman::play(ULONGLONG timeUsed)
{
	Movable::play(timeUsed);
	if (isDead()) {
		if (getAnimation()->isEnded())
			Game::instance()->changeScene(Game::SCENE_GAMEOVER);
	}
}

void Pacman::kill()
{
	Movable::kill();
	getAnimation()->use(ACTION_DEAD_DOWN);
}

void Pacman::setMap(Map *map)
{
	Movable::setMap(map);

	POINT p;
	POINTFLOAT pf;
	getMap()->getPacmanStartPosition(&p, &pf);
	setXY(p.x, p.y);
	getAnimation()->moveTo(pf.x, pf.y);
}

