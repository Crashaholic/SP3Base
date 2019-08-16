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
	void explosionResponse(GameObject * go);

	void playerDeath(GameObject* go);

	GameObject* fetchGO();
	std::vector<GameObject*>& getlist();
	void addGO(GameObject* go);

	void cleanList();
	bool testSAT(Vector3 axis, Vector3 corn[], float &min, float &max);
	bool overlap(float min1, float max1, float min2, float max2);
	void updateCorn(GameObject* go);

	int lives;			// Replaces UPGRADE_3 in collision response (++lives)
	int upgrade_1;		// Blast radius OR Reload speed
	int upgrade_2;		// Bomb count OR Movement speed
	int attackCount;	// No. of times the player has fired a weapon (bursts count as one)
	int kills;
	float accuracy;
	int highScore;

	Terrain* terreference;

	bool tankup1, tankup2, tankup3;
	bool tUpgrade;
	int tlives;
	int random, tUp;
};

#endif