// Ryan
#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"
#include <string>

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

private:
	//Mesh* meshList[NUM_GEOMETRY];
	Camera camera;
	float fps;

	// Buttons
	Button* bArray[5];

	// Text
	string sArray[5];
	string sArrayI[5];
	string sArrayA[5];
	bool muted;
	int choice;
};

#endif