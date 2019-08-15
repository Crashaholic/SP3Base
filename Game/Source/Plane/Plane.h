#ifndef PLANE_H
#define PLANE_H


//#include "GameObject.h"
//#include "Vector3.h"
#include "../Weapon/Bomb.h"
#include <Windows.h>


class Plane : public GameObject
{
private:
	float topSpeed;
	float turnSpeed;
	int maxProjectiles;
protected:
	virtual void Primary();
	virtual void Secondary();
public:
	Plane();
	~Plane();
	virtual void Update(double dt);
	void ReadInput(double dt, char left, char right, char pri, char sec);
	virtual void Init();
};

#endif