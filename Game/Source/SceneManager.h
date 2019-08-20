// Ryan
#pragma once
#include "Scene.h"
#include <unordered_map>
#include <fstream>

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

	static int tankChoice;
	static std::string tankDecalChoice;
	static Color tankColor[2];

	// Money unlock system
	static int money;
	static bool warthog;
	static bool harrier;
	void readMonies();
	void writeMonies(int money, bool warthog, bool harrier);
};

