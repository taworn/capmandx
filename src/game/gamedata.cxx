/**
 * @file gamedata.cxx
 * @desc Game data module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "../game.hxx"
#include "common.hxx"

GameData *GameData::singleton = NULL;

GameData::~GameData()
{
	singleton = NULL;
}

GameData::GameData()
{
	assert(singleton == NULL);
	singleton = this;
}

void GameData::clear()
{
	divoList.clear();
}
