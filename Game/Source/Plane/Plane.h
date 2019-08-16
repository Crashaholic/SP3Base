#ifndef PLANE_H
#define PLANE_H


//#include "GameObject.h"
//#include "Vector3.h"

#include "../GOManager.h"
#include "Vector3.h"
#include <vector>

class Plane : public GameObject
{
protected:
	float topSpeed;
	float turnSpeed;
	int maxProjectiles;
	int priAmmo;
	int secAmmo;
protected:
	virtual void Primary();
	virtual void Secondary();
	std::vector<GameObject*> priprojectiles;
public:
	Plane();
	~Plane();
	virtual void Init();
	virtual void AddPri(int num);
	virtual void AddSec(int num);
	virtual void Update(double dt);
	void ReadInput(double dt, char left, char right, char pri, char sec);
};

#endif