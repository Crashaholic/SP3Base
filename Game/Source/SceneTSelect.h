// Ryan
#ifndef SCENE_TSELECT_H
#define SCENE_TSELECT_H
#define NUM_TBUTTON 6
#define NUM_TSLIDER 6
#define MAX_TDECALS 0
#define MAX_TANKS 1

#include "Scene.h"

class SceneTSelect : public Scene
{
public:
	SceneTSelect();
	~SceneTSelect();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	float fps;

	Color tankColor[2];
	// Buttons
	Button* bArray[NUM_TBUTTON];
	std::string sArray[NUM_TBUTTON];
	// 0: Start
	// 1: Prev Tank(No Effect at the moment)
	// 2: Next Tank(No Effect at the moment)
	// 3: Prev Decal
	// 4: Next Decal
	// 5: Back To Main

	Button* slArray[NUM_TSLIDER];

	int choice;

	int currentTank;
	int currentDecal;

	unsigned int decals[MAX_TANKS][MAX_TDECALS + 1];
	int tanks[MAX_TANKS];
};

#endif