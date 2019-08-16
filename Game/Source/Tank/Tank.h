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

	virtual void Init();
	virtual void Update(double dt);
	virtual void move_leftright();
	virtual void SetGORef(GameObject* GOref);
};

