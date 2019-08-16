#pragma once

#include "../../Source/Plane/Plane.h"
#include "../EnemyBase.h"

class GameObject;

class PlaneEnemy : public Plane, public EnemyBase
{
public:
	PlaneEnemy();
	~PlaneEnemy();
};

