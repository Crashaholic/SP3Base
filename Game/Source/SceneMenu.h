// Ryan
#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"
#include <string>
#include "HighscoreSystem.h"

using namespace std;

class SceneMenu : public Scene
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//bool sound;
private:
	Camera camera;
	float fps;

	// Buttons
	Button* bArray[5];

	// Text
	string sArray[5];
	int choice;

	std::vector<Score> scoresPlane;
	std::vector<Score> scoresTank;
};

#endif