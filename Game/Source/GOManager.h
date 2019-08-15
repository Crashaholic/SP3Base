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

	bool collisionGate(GameObject* go1, GameObject* go2);
	bool checkCollision(GameObject* go1, GameObject* go2);
	void collisionResponse(GameObject* go1, GameObject* go2);

	bool terrainGate(GameObject * go);
	bool checkTerrain(GameObject* go);
	void terrainResponse(GameObject* go);

	void playerDeath(GameObject* go);

	GameObject* fetchGO();
	std::vector<GameObject*>& getlist();
	void addGO(GameObject* go);

	void cleanList();
	bool testSAT(Vector3 axis, Vector3 corn[], float &min, float &max);
	bool overlap(float min1, float max1, float min2, float max2);
	void updateCorn(GameObject* go);

	int lives;			// No. of lives
	int upgrade_1;		// Blast radius OR Reload speed
	int upgrade_2;		// Bomb count OR Movement speed
	int upgrade_3;		// Extra life
	int attackCount;	// No. of times the player has fired a weapon (bursts count as one)
	int kills;			// No. of enemy deaths
	float accuracy;		// kills / attackCount
	int highScore;		// kills + (accuracy / 10) * kills

	Terrain* terreference;
};

#endif