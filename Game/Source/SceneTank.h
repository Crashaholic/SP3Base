// Ryan
#ifndef SCENE_TANK_H
#define SCENE_TANK_H

#include "Scene.h"

class SceneTank : public Scene
{
public:
	SceneTank();
	~SceneTank();

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
	GameObject *m_ghost;
};

#endif