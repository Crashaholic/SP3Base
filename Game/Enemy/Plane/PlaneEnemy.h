#pragma once

#include "../../Source/Plane/Plane.h"
#include "../EnemyBase.h"

struct GameObject;

class PlaneEnemy : public Plane, public EnemyBase
{
public:
	PlaneEnemy();
	~PlaneEnemy();
};

