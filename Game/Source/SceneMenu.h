// Ryan
#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"

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

	// Physics
	float m_speed;
	Vector3 m_gravity;

	// Buttons
	Button* playPlane;
	Button* playTank;
	Button* play2P;
	Button* highScore;
	Button* mute;
};

#endif