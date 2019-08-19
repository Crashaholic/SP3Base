#pragma once

#include "../../Tank/Tank.h"
#include "../EnemyBase.h"

struct GameObject;

class TankEnemy : public Tank, public EnemyBase
{
public:
	TankEnemy();
	~TankEnemy();
};

