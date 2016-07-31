#include <windows.h>
#include <boost/log/trivial.hpp>
#include "scene.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene()
{
	init();
}

void Scene::reset()
{
	fini();
	init();
}

void Scene::init()
{
}

void Scene::fini()
{
}

void Scene::fps(ULONGLONG timeCurrent)
{
	frameCount++;
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		ULONGLONG fps = frameCount * 1000 / timeUsage;
		timeStart = timeCurrent;
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

