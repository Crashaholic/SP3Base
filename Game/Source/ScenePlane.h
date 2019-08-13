#ifndef SCENE_PLANE_H
#define SCENE_PLANE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "shader.hpp"
#include "Terrain.h"
#include <vector>

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
	Camera camera;

	float fps;

	//Physics
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 m_gravity;
	GameObject *m_ghost;
	GameObject *m_timeGO;
	float m_timeEstimated1;
	float m_timeTaken1;
	float m_timeEstimated2;
	float m_timeTaken2;
	float m_heightEstimated;
	float m_heightMax;
	Terrain terr;

	unsigned decal1;
};

#endif