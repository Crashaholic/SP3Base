// Ryan
#pragma once
#include "Scene.h"
#include <unordered_map>
#include "zcustompch.h"

using namespace std;

class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<string, Scene*>sceneList;
	Scene* activeScene;

public:
	SceneManager();
	~SceneManager();

	void firstScene(string name);
	void switchToScene(string name, Scene* curr);
	void addScene(string name, Scene* scene);
	Scene* getActiveScene();
	unordered_map<string, Scene*>& getList();

	static int planeChoice;
	static std::string planeDecalChoice;
	static int decalChoiceP;
	static Color planeColor[2];

	static int tankChoice;
	static std::string tankDecalChoice;
	static int decalChoiceT;
	static Color tankColor[2];

	// Money unlock system
	static int money;
	static bool planeUnlock[3];
	static int planeCost[3];
	void readMonies();
	void writeMonies(int money, bool warthog, bool harrier);
	static SceneManager* getSceneManager();
};

