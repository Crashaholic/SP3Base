#pragma once
#include "TurretTank.h"
class PlayerTank : public TurretTank
{
public:
	PlayerTank();
	~PlayerTank();
	void Init();
	void Update(double dt);
	void move_leftright();
	void SetGORef(GameObject* GOref);
	int tankSpeed;
	double bulletCooldown;
};

