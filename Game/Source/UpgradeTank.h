#ifndef C_UPGRADETANK_H
#define C_UPGRADETANK_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include "shader.hpp"
#include <vector>

class UpgradeTank
{
public:
	UpgradeTank();
	~UpgradeTank();
	void WeaponUp();
	void SpeedUp();
	void HealthUp();

private:
	bool weaponup;
	bool speedup;
	bool sizeup;
};

#endif