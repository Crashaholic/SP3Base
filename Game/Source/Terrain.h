#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"

class Terrain
{
public:

	Terrain();
	~Terrain();

	Vector3 Points[64];

	void GenerateRandomHeight();


};

