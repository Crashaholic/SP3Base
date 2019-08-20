#pragma once
#include "TurretTank.h"
class PlayerTank : public TurretTank
{
public:
	PlayerTank();
	~PlayerTank();
	void Init();
	void Update(double dt);
	void ReadInput(double dt, char left, char right, char aimleft, char aimright, char fire);
};

