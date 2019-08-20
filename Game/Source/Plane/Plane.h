#ifndef PLANE_H
#define PLANE_H


//#include "GameObject.h"
//#include "Vector3.h"

#include "../GOManager.h"
#include "Vector3.h"
#include <vector>
struct GameObject;

class Plane 
{
protected:
	float topSpeed;
	float turnSpeed;
	//int maxProjectiles;
	int priAmmo;
	int secAmmo;
protected:
	virtual void Primary();
	virtual void Secondary();
	std::vector<GameObject*> priProjectiles;
public:
	virtual void SetGORef(GameObject* GOref);

	GameObject* GOref;
	Plane();
	~Plane();
	virtual void Init();
	virtual void AddPri(int num);
	virtual void AddSec(int num);
	virtual void Update(double dt);
	virtual void Turn_CWCCW(double dt, bool CCW);
	void ReadInput(double dt, char left, char right, char pri, char sec);
	int getPri();
	int getSec();
};

#endif