#ifndef SCENE_PLANE_H
#define SCENE_PLANE_H

#include "Scene.h"
#include "Terrain.h"
#include "Plane/Plane.h"

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

private:
	Plane *plane;
	Camera camera;

	float fps;


	//=== Wave ===
	unsigned int waveNo;
	unsigned int enemyCount;
	unsigned int startCount;
	void EndWave();
	void SpawnEnemy();
	vec3 SpawnPos1;
	vec3 SpawnPos2;
	double spawnTimer;


	// Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	Vector3 m_gravity;
	Terrain terr;

	unsigned decal1;



	GameObject *tank, *tank2;
	float angle;
	float tankSpeed;
	double bulletCooldown;
};

#endif