#include "zcustompch.h"
#ifndef GO_MANAGER_H
#define GO_MANAGER_H

#include "GameObject.h"

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
	void toExplosion(GameObject* go, bool fromPlayer);
	void exResponse(GameObject* go, bool fromPlayer);

	GameObject* fetchGO();
	GameObject* fetchReservedGO();
	std::vector<GameObject*>& getlist();
	void addGO(GameObject* go);

	void cleanList();
	bool testSAT(Vector3 axis, Vector3 corn[], float &min, float &max);
	bool overlap(float min1, float max1, float min2, float max2);
	void updateCorn(GameObject* go);

	void enemyDeath(GameObject* go);

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

	unsigned int planeKills;
	unsigned int tankKills;
	unsigned int totalShots;
	unsigned int totalHits;
	float planeAccuracy;
	float tankAccuracy;
	unsigned int planeHighscore;
	unsigned int tankHighscore;

	bool wind;								// activates wind
	double windBT;							// bouncetime to regulate time between direction randomisations (WIND_TIMER)
	Vector3 windVector;						// current vector of wind
	Vector3 windVectorN;					// future vector of wind
	const float WIND_POWER = 5.0f;
	const float WIND_TIMER = 10.0f;
	const float WIND_RANGE = 90.0f;			// range of wind randomisation

	const float UPGRADE_CHANCE = 20.0f;		// chance of upgrade dropping from enemies

	bool rain;
	const float RAIN_CHANCE = 50.0f;		// chance of rain occuring
	static const int RAIN_INTENSITY = 50;	// number of rain objects

	// These values should only be used for sceneEnd to identify which data to display
	enum STYPE
	{
		NONE = 0,
		FROM_PLANE,
		FROM_TANK,
		FROM_2P,
		TOTAL
	};
	STYPE sceneID;
	
	Terrain* terreference = nullptr;

	const Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);

	bool muted;
	void playSound(std::string name);

	bool checkForEnemy();
};

#endif