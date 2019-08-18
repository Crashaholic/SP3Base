#include "SceneManager.h"

int				SceneManager::planeChoice;
std::string	SceneManager::planeDecalChoice;
Color			SceneManager::planeColor[2];
int				SceneManager::tankChoice;
std::string	SceneManager::tankDecalChoice;
Color			SceneManager::tankColor[2];


SceneManager::SceneManager()
{
	planeChoice = 0;
	planeColor[0].Set(1, 1, 1);
	planeColor[1].Set(1, 1, 1);
	planeDecalChoice = "";

	tankColor[0].Set(1, 1, 1);
	tankColor[1].Set(1, 1, 1);
	tankDecalChoice = "";
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
