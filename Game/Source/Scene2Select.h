#ifndef SCENE_2SELECT_H
#define SCENE_2SELECT_H
#define MAX_2PDECALS 2
#define MAX_2PLANES 1
#define NUM_2PBUTTON 6
#define NUM_2PSLIDER 6
#define NUM_2TBUTTON 4
#define NUM_2TSLIDER 6
#define MAX_2TDECALS 0
#define MAX_2TANKS 1

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
	Button* bPArray[NUM_2PBUTTON];
	std::string sPArray[NUM_2PBUTTON];
	Button* slPArray[NUM_2PSLIDER];

	Button* bTArray[NUM_2TBUTTON];
	std::string sTArray[NUM_2TBUTTON];
	Button* slTArray[NUM_2TSLIDER];

	int choice1;
	int choice2;

	int currentPlane;
	int currentDecal1;
	int currentTank;
	int currentDecal2;

	unsigned int decals[3][MAX_2PDECALS + 1];
	int planes[MAX_2PLANES];
	int tanks[MAX_2TANKS];
};

#endif