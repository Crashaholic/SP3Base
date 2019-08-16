#pragma once
#include "Tank.h"

struct GameObject;

class TurretTank : public Tank
{
protected:
	float turretAngle;

public:
	TurretTank();
	~TurretTank();


	virtual void Init();
	//virtual void Update(double dt) = 0;
};

