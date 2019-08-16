#ifndef GO_MANAGER_H
#define GO_MANAGER_H

#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"
#include <vector>

class GOManager:public Singleton<GOManager>
{
	friend Singleton<GOManager>;

private:
	std::vector<GameObject*> m_goList;

public:
	GOManager();
	~GOManager();
	void init();
	void update(double dt);

	// Gates collision so that:
	// > go1 is always Player / Player Projectile / Enemy Projectile
	// > go2 is always a correct corresponding possible collision object (Refer to collision diagram)
	bool collisionGate(GameObject* go1, GameObject* go2);
	bool checkCollision(GameObject* go1, GameObject* go2);
	void collisionResponse(GameObject* go1, GameObject* go2);

	bool terrainGate(GameObject * go);
	bool checkTerrain(GameObject* go);
	void terrainResponse(GameObject* go);

	void planeDeath(GameObject* go);
	void tankDeath(GameObject* go);
	void toExplosion(GameObject* go);
	void exResponse(GameObject* go);

	GameObject* fetchGO();
	std::vector<GameObject*>& getlist();
	void addGO(GameObject* go);

	void cleanList();
	bool testSAT(Vector3 axis, Vector3 corn[], float &min, float &max);
	bool overlap(float min1, float max1, float min2, float max2);
	void updateCorn(GameObject* go);

	// Replaces UPGRADE_3 in collision response
	int planeLives;
	int tankLives;
	
	// Komet	: Bomb count
	// A10		: Bullets per burst
	// Tank		: Reload speed
	int upgrade_1;

	// Komet	: Nuke count
	// A10		: Missile count
	// Tank		: Movement speed
	int upgrade_2;		

	// No. of times the player has fired a weapon (bursts count as one)
	int attackCount;

	int planeKills;
	int tankKills;
	float planeAccuracy;
	float tankAccuracy;
	int planeHighscore;
	int tankHighscore;

	Terrain* terreference;

	bool tankup1, tankup2, tankup3;
	bool tUpgrade;
	int tlives;
	int random, tUp;
};

#endif