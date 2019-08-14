#ifndef SCENE_PLANE_H
#define SCENE_PLANE_H

#include "Scene.h"
#include "Terrain.h"
#include "Plane/Plane.h"

class ScenePlane : public Scene
{


public:
	ScenePlane();
	~ScenePlane();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


private:
	Plane *plane;
	Camera camera;

	float fps;

	// Physics
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 m_gravity;
	Terrain terr;

	unsigned decal1;
};

#endif