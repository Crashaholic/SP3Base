#pragma once
#include "Vector3.h"
#include "MeshBuilder.h"

class Terrain
{
public:

	Terrain();
	~Terrain();

	Vector3 Points[64];
	Vector3 Bottom[64];

	Mesh* tMesh;



	void GenerateRandomHeight(unsigned int worldWidth);
	void GenerateTerrainMesh();

};

