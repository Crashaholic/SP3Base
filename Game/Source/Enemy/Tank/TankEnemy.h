#pragma once

#include "../../Tank/Tank.h"
#include "../EnemyBase.h"

struct GameObject;

class TankEnemy : public Tank, public EnemyBase
{
public:
	TankEnemy();
	TankEnemy(vec3 pos, GameObject* ref, float m_worldWidth);
	~TankEnemy();

	// ref is Player's GO reference
	void SpawnNewTankEnemy(vec3 pos, GameObject* ref, float m_worldWidth);
	void Update(double dt) override;
	void Move_LeftRight(double dt, bool left) override;

	unsigned int Range = 5;


	//Stage 1 = Enter Scene
	//Stage 2 = Find Player
	//Stage 3 = Move Close To Player
	//Stage 5 = Aim
	//Stage 6 = Fire! (loop back to 2)
	int Stage;
	
};

