#ifndef  Harrier_H
#define Harrier_H

#include "Plane.h"

class Harrier : public Plane
{
protected:
	bool VTOLmode;
	const double reloadSpeed = 0.025;
	const double sprayRecoverTime = 1.0;
	const float inaccuracy = 5.0f;
	float VTOLTopSpeed = 9.8f;
	bool fire;
	int currentSpray;
	double reload;
	Vector3 up;
public:
	Harrier();
	~Harrier();
	void Primary();
	void Secondary();
	virtual void Update(double dt);
	void Move_UpDown(double dt, bool up);
	virtual void Init();
	void ReadInput(double dt, char left, char right, char pri, char sec, char up, char down, char vtol, char flip);
};

#endif