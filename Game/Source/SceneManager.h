#pragma once
#include "Scene.h"
#include <unordered_map>

using namespace std;

class SceneManager
{
private:
	unordered_map<string, Scene*>sceneList;
	Scene* activeScene;

public:
	SceneManager();
	~SceneManager();

	void switchToScene(string name);
	void addScene(string name, Scene* scene);

};

