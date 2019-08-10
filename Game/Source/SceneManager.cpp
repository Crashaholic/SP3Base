#include "SceneManager.h"



SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::switchToScene(string name)
{
	this->activeScene = sceneList[name];
}

void SceneManager::addScene(string name, Scene * scene)
{
	if ((scene == NULL) || (scene == nullptr))
	{
		return;
	}
	this->sceneList[name] = scene;
}
