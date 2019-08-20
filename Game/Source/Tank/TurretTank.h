#pragma once
#include "Tank.h"

struct GameObject;

class TurretTank : public Tank
{
protected:
	float turretAngle;
	float turretOffset;
public:
	TurretTank();
	~TurretTank();
	virtual void Turret_LeftRight(double dt, bool left);

	virtual void Init();
	//virtual void Update(double dt) = 0;
};

