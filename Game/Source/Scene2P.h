#ifndef SCENE_2P_H
#define SCENE_2P_H

#include "Scene.h"
#include "Terrain.h"
//Include All planes here

class Scene2P : public Scene
{
	static const int MAX_SPEED = 20;
	static const int BULLET_SPEED = 50;
	static const unsigned int SPAWNTIMER = 3;
	static const unsigned int STARTINGCOUNT = 3 /*MINUS 1*/- 1;
	static const unsigned int ENEMYSPAWNCHNCRANGE_MIN = 0;
	static const unsigned int ENEMYSPAWNCHNCRANGE_MAX = 2;
	static const unsigned int ENEMYSPAWNCHNC = 1;

public:
	Scene2P();
	~Scene2P();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Plane* plane;
	Tank* player;
	Terrain* terr;

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
	float m_speed;

	unsigned decal1;


	//GameObject *tank, *tank2;
	float angle;
	float tankSpeed;
	double bulletCooldown;
	bool tUpgrade;
};

#endif