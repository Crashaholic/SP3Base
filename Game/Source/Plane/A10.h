#ifndef  A10_H
#define A10_H

#include "Plane.h"

class A10 : public GameObject
{
protected:
	void Primary();
	void Secondary();
public:
	A10();
	~A10();
	//virtual void Update(double dt);
	//void ReadInput(double dt, char left, char right, char pri, char sec);
	//virtual void Init();
	//void Reset();
	//void SetTerrain(Terrain* terr);
};

#endif