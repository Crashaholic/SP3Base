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
	static const int MAX_SPEED = 20;
	static const int BULLET_SPEED = 50;

public:
	ScenePlane();
	~ScenePlane();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	GameObject* FetchGO();

private:
	Camera camera;

	float fps;

	// Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 m_gravity;
	Terrain terr;

	unsigned decal1;

	GameObject *tank, *tank2;
	float angle;
	double bulletCooldown;
};

#endif