// Ryan
#ifndef SCENE_PSELECT_H
#define SCENE_PSELECT_H
#define MAX_DECALS 2
#define MAX_PLANES 2
#define NUM_BUTTON 6
#define NUM_SLIDER 6
#include "Scene.h"

class ScenePSelect : public Scene
{
public:
	ScenePSelect();
	~ScenePSelect();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	float fps;
	Color planeColor[2];
	// Buttons
	Button* bArray[NUM_BUTTON];
	std::string sArray[NUM_BUTTON];
	// 0: Start
	// 1: Prev Plane
	// 2: Next Plane
	// 3: Prev Decal
	// 4: Next Decal
	// 5: Back To Main

	Button* slArray[NUM_SLIDER];
	
	int choice;

	int currentPlane;
	int currentDecal;

	unsigned int decals[MAX_PLANES][MAX_DECALS + 1];
	int planes[MAX_PLANES];

};

#endif