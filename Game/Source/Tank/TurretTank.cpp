#include "TurretTank.h"

TurretTank::TurretTank()
{
}

TurretTank::~TurretTank()
{
}

void TurretTank::Init()
{
	Tank::Init();
	turretAngle = 0.0f;
}
