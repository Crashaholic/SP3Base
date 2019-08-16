#ifndef  A10_H
#define A10_H

#include "Plane.h"

class A10 : public Plane
{
protected:
	void Init();
	void Primary();
	void Secondary();
	void Update(double dt);
public:
	A10();
	~A10();
private:
	const double reloadSpeed = 0.05;
	const double sprayRecoverTime = 1.0;
	const float inaccuracy = 5.0f;
	bool fire;
	int currentSpray;
	double reload;
	//virtual void Update(double dt);
	//void ReadInput(double dt, char left, char right, char pri, char sec);
	//virtual void Init();
	//void Reset();
	//void SetTerrain(Terrain* terr);
};

#endif