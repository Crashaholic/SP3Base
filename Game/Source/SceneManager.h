// Ryan
#pragma once
#include "Scene.h"
#include <unordered_map>

using namespace std;

class SceneManager
{
private:
	unordered_map<string, Scene*>sceneList;
	Scene* activeScene;

	SceneManager();											// prevent multiple
	SceneManager(const SceneManager& manager);				// prevent copy
	SceneManager& operator=(const SceneManager& manager);	// prevent assign

public:
	~SceneManager();

	void firstScene(string name);
	void switchToScene(string name, Scene* curr);
	void addScene(string name, Scene* scene);
	static SceneManager& getSceneManager();
	Scene* getActiveScene();
	unordered_map<string, Scene*>& getList();

	static int planeChoice;
	static std::string planeDecalChoice;
	static Color planeColor[2];

	static std::string tankDecalChoice;
	static Color tankColor[2];
};

