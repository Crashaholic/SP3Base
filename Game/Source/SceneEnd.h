// Ryan
#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"

class SceneEnd : public Scene
{
public:
	SceneEnd();
	~SceneEnd();

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