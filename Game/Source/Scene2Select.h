// Ryan
#ifndef SCENE_2SELECT_H
#define SCENE_2SELECT_H
#define MAX_PDECALS 2
#define MAX_PLANES 2
#define NUM_PBUTTON 6
#define NUM_PSLIDER 6
#define NUM_TBUTTON 6
#define NUM_TSLIDER 6
#define MAX_TDECALS 0
#define MAX_TANKS 1

#include "Scene.h"
#include "SceneMenu.h"

class Scene2Select : public Scene
{
public:
	Scene2Select();
	~Scene2Select();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//Mesh* meshList[NUM_GEOMETRY];

	Camera camera;
	float fps;

	Color planeColor[2];
	Color tankColor[2];

	// Buttons
	Button* bPArray[NUM_PBUTTON];
	std::string sPArray[NUM_PBUTTON];
	Button* slPArray[NUM_PSLIDER];

	Button* bTArray[NUM_TBUTTON];
	std::string sTArray[NUM_TBUTTON];
	Button* slTArray[NUM_TSLIDER];

	int choice1;
	int choice2;

	int currentPlane;
	int currentDecal1;
	int currentTank;
	int currentDecal2;

	unsigned int decals[3][MAX_PDECALS + 1];
	int planes[MAX_PLANES];
	int tanks[MAX_TANKS];

	SceneMenu menu;
};

#endif