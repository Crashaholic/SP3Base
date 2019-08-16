#pragma once
#include "Tank.h"
class PlayerTank : public Tank
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

