#pragma once
#include "../GOManager.h"

struct GameObject;

class Tank
{
protected:
	float heightOffset;
public:
	Tank();
	~Tank();

	GameObject* GOref;

	void SetGORef(GameObject* GOref);
	float tankSpeed;
	double bulletCooldown;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Move_LeftRight(double dt, bool left);
	virtual void Fire();
};

