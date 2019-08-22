// Ryan
#ifndef SCENE_TANK_H
#define SCENE_TANK_H

#include "Scene.h"
#include "Enemy/Plane/PlaneEnemy.h"

class SceneTank : public Scene
{
private:
	static const unsigned int SPAWNTIMER = 3;
	static const unsigned int STARTINGCOUNT = 3 /*MINUS 1*/ - 1;
	static const unsigned int ENEMYSPAWNCHNCRANGE_MIN = 0;
	static const unsigned int ENEMYSPAWNCHNCRANGE_MAX = 2;
	static const unsigned int ENEMYSPAWNCHNC = 1;
public:
	SceneTank();
	~SceneTank();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//Mesh* meshList[NUM_GEOMETRY];
	Tank* player;
	Terrain* terr;

	Camera camera;

	float fps;

	//=== Wave ===
	unsigned int waveNo;
	unsigned int enemyCount;
	unsigned int startCount;
	void EndWave();
	bool SpawnEnemy();
	vec3 SpawnPos1;
	vec3 SpawnPos2;
	double spawnTimer;

	// Physics
	float m_speed;

	std::vector<PlaneEnemy*> enemyList;

};

#endif