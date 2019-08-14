// Ryan
#ifndef SCENE_PSELECT_H
#define SCENE_PSELECT_H

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
	//Mesh* meshList[NUM_GEOMETRY];

	Camera camera;

	float fps;

	// Physics
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 m_gravity;
	GameObject *m_ghost;
};

#endif