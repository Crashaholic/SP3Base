#include "SceneManager.h"



SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	// WIP
	// unordered_map<string, Scene*> empty;
	// using std::swap;
	// swap(sceneList, empty);
}

void SceneManager::firstScene(string name)
{
	activeScene = sceneList[name];
	activeScene->Init();
}

void SceneManager::switchToScene(string name, Scene* curr)
{
	curr->readyExit = true;
	activeScene = sceneList[name];
}

void SceneManager::addScene(string name, Scene * scene)
{
	if ((scene == NULL) || (scene == nullptr))
	{
		return;
	}
	this->sceneList[name] = scene;
}

SceneManager & SceneManager::getSceneManager()
{
	static SceneManager manager;
	return manager;
}

Scene * SceneManager::getActiveScene()
{
	return activeScene;
}

unordered_map<string, Scene*>& SceneManager::getList()
{
	return sceneList;
}
