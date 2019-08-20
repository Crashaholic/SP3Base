#pragma once

#include "../../Tank/Tank.h"
#include "../EnemyBase.h"

struct GameObject;

class TankEnemy : public Tank, public EnemyBase
{
public:
	TankEnemy();
	TankEnemy(vec3 pos, GameObject* ref);
	~TankEnemy();

	// ref is Player's GO reference
	void SpawnNewTankEnemy(vec3 pos, GameObject* ref, float m_worldWidth);
	void Update(double dt) override;
};

