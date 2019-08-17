// Ryan
#ifndef SCENE_2SELECT_H
#define SCENE_2SELECT_H

#include "Scene.h"

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

	// Physics
	float m_speed;
	Vector3 m_gravity;
	GameObject *m_ghost;
};

#endif