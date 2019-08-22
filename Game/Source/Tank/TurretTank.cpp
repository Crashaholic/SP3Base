#include "zcustompch.h"
#include "TurretTank.h"

TurretTank::TurretTank()
{
}

TurretTank::~TurretTank()
{
}

void TurretTank::Turret_LeftRight(double dt, bool left)
{
	if (left)
		turretAngle += 3.0f * static_cast<float>(dt);
	else
		turretAngle -= 3.0f *static_cast<float>(dt);

}

void TurretTank::Init()
{
	Tank::Init();
	turretAngle = 0.0f;
}
